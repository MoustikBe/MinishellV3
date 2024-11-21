#include "minishell.h"

void    sigint_handler(int sig) //IRIS (sigint_handler and manage_signals function)
{
    (void)sig;
    write(1, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
}

void    manage_signals(void)
{
    signal(SIGQUIT, SIG_IGN); //IRIS (sig_ign ignores the "\"" signal, i added the signal stuff in main)
    signal(SIGINT, &sigint_handler);
}

void cmd_cleaner(t_shell *shell)
{
	int i; 
	int mem;
	int j;
	char *pipe_cmd;


	i = 0;
	j = 0;
	mem = 0;
	// RECONNAITRE SI LA COMMANDE EST pwd| pwd pour transfo en pwd | pwd
	// OU ENCORE SI C'EST pwd|pwd pour transfo en pwd | pwd

	// RECOPIER LA COMMANDE A L'IDENTIQUE ET A CHAQUE FOIS QUE UN PIPE EST DETECTER 
	// AJOUTER UN ESPACE DEVANT ET UN DERRIERE
	// DONC POUR CHAQUE PIPE FAIRE + 2 EN ALLOC DE MEMOIRE.

	while(shell->cmd[i])
	{
		if(shell->cmd[i] == '<' && shell->cmd[i + 1] == ' ' || shell->cmd[i] == '>' && shell->cmd[i + 1] == ' ')
		{
			i++;
			while(shell->cmd[i] == ' ')
			{
				i++;
				mem--;
			}
		}
		else if(shell->cmd[i] == '|')
		{
			if(shell->cmd[i  - 1] != ' ')
				mem++;
			if(shell->cmd[i + 1] != ' ')
				mem++;
			mem++;
			i++;
		}
		else
		{
			i++;
			mem++;
		}
	}
	if(shell->cmd[i - 1] != ' ')
		mem++;
	pipe_cmd = malloc(sizeof(char) * mem + 1);
	i = 0; 
	while(shell->cmd[i])
	{
		if(shell->cmd[i] == '<' && shell->cmd[i + 1] == ' ' || shell->cmd[i] == '>' && shell->cmd[i + 1] == ' ')
		{
			pipe_cmd[j] = shell->cmd[i];
			j++;
			while(shell->cmd[++i] == ' ');
		}
		else if(shell->cmd[i] == '|')
		{
			if(shell->cmd[i  - 1] == ' ')
			{
				pipe_cmd[j] = '|';
				j++;
			}
			else 
			{
				pipe_cmd[j] = ' ';
				pipe_cmd[j + 1] = '|';
				j = j + 2;
			}
			if(shell->cmd[i + 1] != ' ')
			{
				pipe_cmd[j] = ' ';
				j++;
			}
			i++;
		}
		else
		{
			pipe_cmd[j] = shell->cmd[i];
			i++;
			j++;
		}
	}
	if(shell->cmd[i - 1] != ' ')
	{
		pipe_cmd[j] = ' ';
		j++;
	}
	pipe_cmd[j] = '\0';
	//printf("%s\n", pipe_cmd);
	shell->cmd = ft_strdup(pipe_cmd);
}


int search_in_env_writed(t_shell *shell, char *cmp_cmd, int temp_fd)
{
	t_env *env_v;
	char *cmp_cmd_2;
	int i = 0;

	env_v = shell->env;
	while (env_v)
	{
		cmp_cmd_2 = build_cmp(env_v->env_var);
		//printf("cmp_1 -> %s cmp_2 -> %s\n", cmp_cmd, cmp_cmd_2);
		if(str_cmp(cmp_cmd, cmp_cmd_2) == 1)
		{
			while (env_v->env_var[i] != '=')
				i++;
			i++;
			while (env_v->env_var[i])
			{
			
				ft_putchar_fd(env_v->env_var[i], temp_fd);
				i++;
			}
			return(ft_strlen(env_v->env_var) - ft_strlen(cmp_cmd_2));
		}
		free(cmp_cmd_2);
		env_v = env_v->next;
	}
	return(0);
}

void heredoc_expansion(t_shell *shell, char *gnl_val, int temp_fd)
{
	// IL FAUT PARCOURIR LA shell->cmd ET detecter si jamais il y'a une variable d'env.
	// SA detecte les dolars si on envois un char * -> check_cmd_quotes();
	// FAIRE UN SPLIT de la cmd. Si la fonction renvois 10. -> il faut changer la commande. 
	
	// TENTATIVE // 
	int i = 0;
	int i_copy = 0;
	int j = 0;
	int len = 0;
	int ret_val = 0;
	int flag = 0;
	char *cmp_cmd;

	// CONNAITRE la len de la nouvelle commande. donc
	while (gnl_val[i])
	{
		// TROUVER UN MOYEN DE SAVOIR SI IL S'AGIT DE $?
		if(gnl_val[i] == '$')
		{
			if(gnl_val[i + 1] == '?' && gnl_val[i + 2] == ' ')
				return ;
			flag = 1;
			i++;
			i_copy = i;
			while (gnl_val[i] && gnl_val[i] != ' ' && gnl_val[i_copy] != '\n')
			{
				i++;
				j++;
			}
			cmp_cmd = malloc(sizeof(char) * j + 1);
			j = 0;
			while (gnl_val[i_copy] && gnl_val[i_copy] != ' ' && gnl_val[i_copy] != '\n')  
			{
				cmp_cmd[j] = gnl_val[i_copy];
				j++;
				i_copy++;
			}
			cmp_cmd[j] = '\0';
			// PARFAITE CMP_CMD DECOUPER 
			// MNT essaye de trouver une correspondance et si il y'a une correspondance 
			// RENVOYER 1 SI PAS 0
			//printf("cmp_cmd -> %s\n", cmp_cmd);
			len = len + search_in_env_writed(shell, cmp_cmd, temp_fd);
			//printf(" len_search -> %d\n", search_in_env(shell, cmp_cmd));
		}
		else
		{
		//	printf("char -> %c\n", shell->cmd[i]);
			ft_putchar_fd(gnl_val[i], temp_fd);
			len++;
			i++;
		}
	}
	ft_putchar_fd('\n', temp_fd);
}

int check_env_var(char *str)
{
	int i;

	while (str[i])
	{
		if(str[i] == '$')
			return(1);
		i++;
	}
	return(0);
}


void exec_herdoc(t_shell *shell)
{
	// Stocker le delimiter
	char *gnl_val;
	char *delemiter;
	int i;
	int l;
	int temp_fd;

	i = 0;
	l = 0;
	while (shell->cmd[i])
	{
		if(shell->cmd[i] == '<' && shell->cmd[i + 1] == '<')
		{
			i = i + 2;
			while (shell->cmd[i] == ' ')
				i++;
			while (shell->cmd[i] && shell->cmd[i] != ' ')
			{
				l++;
				i++;
			}
			break;
		}
		i++;
	}
	
	delemiter = malloc(sizeof(char) * l + 2);
	i = 0;
	l = 0;
	while (shell->cmd[i])
	{
		if(shell->cmd[i] == '<' && shell->cmd[i + 1] == '<')
		{
			i = i + 2;
			while (shell->cmd[i] == ' ')
				i++;
			while (shell->cmd[i] && shell->cmd[i] != ' ')
			{
				delemiter[l] = shell->cmd[i];
				l++;
				i++;
			}
			break;
		}
		i++;
	}
	delemiter[l] = '\n';
	delemiter[l + 1] = '\0';
	//printf("Delimitator -> %s\n", delemiter); //Done V //
	// Comparer la valeur transmise avec la valeur du delimiter
	temp_fd = open("/tmp/.heredoc", O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (temp_fd < 0)
	{
        perror("open");
        exit(EXIT_FAILURE);
    }
	write(0, "> ", 2);
	while (1)
	{
		gnl_val = get_next_line(0);
		if (gnl_val == NULL) //IRIS I used NULL instead of \0 because "cmd[0]" produces a SEGFAULT
		{
			printf("exit\n");
			break;
		}
		// SI valeur == -> STOP
		/* FAIRE UNE SORTE DE MINI EXPANSION DE VARIABLE AFIN DE CHANGER LES VALEUR */
		if(str_cmp(gnl_val, delemiter) == 1)
			break;
		// Il faut expand la variable d'env si il y'en a 
		// Fonction qui detecte si il y'en a 
		if(check_env_var(gnl_val) == 1)
		{
			heredoc_expansion(shell, gnl_val, temp_fd);
			write(0, "> ", 2);
		}
		//	gnl_val = expand_var(shell, gnl_val);
		// Fonction qui change la valeur de gnl_val -> pour gnl_val modifier avec la var
		else
		{
			write(temp_fd, gnl_val, ft_strlen(gnl_val));
			write(0, "> ", 2);
		}
		// SI valeur differentes -> sauvegarder et continuer
		free(gnl_val);
	}
	//free(delemiter);
	//free(gnl_val);
	/*
	
	*/
	

	// UTILISER GNL sur le fd0
	
}

void replace_heredoc(t_shell *shell)
{
	int i = 0;
	int j = 0;
	int len = 0;
	char *new_cmd;
	char *tmp_file;

	tmp_file = ft_strdup("/tmp/.heredoc");
	while (shell->cmd[i])
	{
		if(shell->cmd[i] == '<')
		{
			i = i + 2;
			// ICI je dois faire gaffe que tant que c'est egal a un truc different de espace 
			if(shell->cmd[i] == ' ')
				i++;
			while (shell->cmd[i] != ' ')
				i++;
			//printf("char -> %c\n", shell->cmd[i]);
		}	
		else
		{
			len++;
			i++;
		}
	}

	new_cmd = malloc(sizeof(char) * len + ft_strlen("/tmp/.heredoc") + 1); 
	len = 0;
	i = 0;
	while (shell->cmd[i])
	{
		if(shell->cmd[i] == '<')
		{
			new_cmd[len] = shell->cmd[i];
			len++;
			i = i + 2;
			// ICI je dois faire gaffe que tant que c'est egal a un truc different de espace 
			if(shell->cmd[i] == ' ')
				i++;
			while (shell->cmd[i] != ' ')
				i++;
			while (tmp_file[j])
			{
				new_cmd[len] = tmp_file[j];
				len++;
				j++;
			}
		}	
		else
		{
			new_cmd[len] = shell->cmd[i];
			len++;
			i++;
		}
	}
	new_cmd[len] = '\0';
	shell->cmd = ft_strdup(new_cmd);
}

void here_doc(t_shell *shell)
{
	int i; 

	i = 0;
	while(shell->cmd[i])
	{
		if(shell->cmd[i] == '<' && shell->cmd[i + 1] == '<' && shell->cmd[i + 2] != '\0' && shell->cmd[i + 2] != '<')
		{
			exec_herdoc(shell);
			replace_heredoc(shell);
		}
		i++;
	}

}



int main(int argc, char **argv, char **envp)
{
    char *cmd;
	char *cpy_cmd;
    t_token *token;
    t_shell *shell;
	int ret_val;

	cpy_cmd = NULL;
	shell = malloc(sizeof(t_shell));
	copy_env(envp, shell);
	shell->len_token = 0;
	shell->last_exit_status = 0;
    //token = malloc(sizeof(t_token));
    while (1)
    {
		manage_signals();// ADD FROM IRIS
		shell->error = 0;
        shell->cmd = readline("\033[34;01mMinishell : \033[00m");
		if (shell->cmd == NULL) //IRIS I used NULL instead of \0 because "cmd[0]" produces a SEGFAULT
		{
			printf("exit\n");
			break;
		}
		if(shell->cmd[0] == '\0')
			ret_val = 0;
		else
		{
			cpy_cmd = ft_strdup(shell->cmd);
			cmd_cleaner(shell);
			expansion(shell);
			here_doc(shell);
			ret_val = parsing_main(shell->cmd); //parsing
		}
		
		if(ret_val == 0)
		{
            printf("\033[0;31mMinishell : command not found -> %s\033[00m\n", shell->cmd);
			//
			//break ; // Quand je test mes leaks //
		}
		else if(ret_val == 1)
		{
			// PARSING VALIDER, la commande peut ce faire tokeniser, 
			// AVANT il faut simplement être sure que toute la memoire allouer a été correctement liberer // 
			token = token_main(shell->cmd, token, shell);
			//break;
			if(shell->error == 1)
				printf("\033[0;31mMinishell : command invalid \033[00m\n");
			else
				exec_main(token, shell->cmd, envp, shell);
			//free_all_token(token, len_token(cmd)); //-> IMPORTANT DE FOU, FIX DE LEAK
			//break ;
		}
		else if(ret_val > 1)
		{
			printf("\033[0;31mMinishell : command invalid \033[00m\n");
			//free_all_token(token, len_token(cmd)); //-> IMPORTANT DE FOU, FIX DE LEAK
			//break ; // Quand je test mes leaks //
		}
        add_history(cpy_cmd);
        free(shell->cmd);
		//free(cpy_cmd);*/
    }
	unlink("/tmp/.heredoc");
    free(shell->cmd);
	if(cpy_cmd)
		free(cpy_cmd);
	free(shell);
    //free(token);
    return 0;
}
    // HAVE TO MANAGE //
    // -> echo      (with option -n) parsing -> V token -> V -> exec -> X
    // -> cd        (with only a relative or absolute path) parsing -> V token -> V -> exec -> V
    // -> pwd       (with no options) parsing -> V token -> V -> exec -> V
    // -> export    (with no options) parsing -> V token -> V -> exec -> V
    // -> unset     (with no options) parsing -> V token -> V -> exec -> X
    // -> env       (with no options or arguments) parsing -> V token -> V -> exec -> V
    // -> exit      (with no options) parsing -> V token -> V -> exec -> X
		

// !! TEMPORAL FIX, 11/11/2024 -> Add of a space at the end of the line in the case of 
// a command without space, because was blocking the correct execution of the pipe in certain case


