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
	temp_fd = open(".here_doc_tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
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
		if(str_cmp(gnl_val, delemiter) == 1)
			break;
		write(temp_fd, gnl_val, ft_strlen(gnl_val));
		write(0, "> ", 2);
		// SI valeur differentes -> sauvegarder et continuer
		free(gnl_val);
	}
	//free(delemiter);
	//free(gnl_val);
	/*
	
	*/
	unlink(".here_doc_tmp");

	// UTILISER GNL sur le fd0
	
}

void here_doc(t_shell *shell)
{
	int i; 

	i = 0;
	while(shell->cmd[i])
	{
		if(shell->cmd[i] == '<' && shell->cmd[i + 1] == '<' && shell->cmd[i + 2] != '\0' && shell->cmd[i + 2] != '<')
			exec_herdoc(shell);
		i++;
	}

}



int main(int argc, char **argv, char **envp)
{
    char *cmd;
    t_token *token;
    t_shell *shell;
	int ret_val;

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
			here_doc(shell);
			cmd_cleaner(shell);
			expansion(shell);
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
        add_history(shell->cmd);
        free(shell->cmd);
    }
    free(shell->cmd);
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
