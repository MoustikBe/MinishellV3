#include "minishell.h"

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
	pipe_cmd = malloc(sizeof(char) * mem + 2);
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
	pipe_cmd[j] = ' ';
	pipe_cmd[j + 1] = '\0';
	//printf("%s\n", pipe_cmd);
	shell->cmd = ft_strdup(pipe_cmd);
}




int main(int argc, char **argv, char **envp)
{
    char *cmd;
    t_token *token;
    t_shell *shell;
	int ret_val;

	shell = malloc(sizeof(t_shell));
	shell->len_token = 0;
    //token = malloc(sizeof(t_token));
    while (1)
    {
		shell->error = 0;
        shell->cmd = readline("\033[34;01mMinishell : \033[00m");
		if(shell->cmd[0] == '\0')
			ret_val = 0;
		else
		{
			cmd_cleaner(shell);
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
			free_all(token, len_token(cmd)); //-> IMPORTANT DE FOU, FIX DE LEAK
			//break ;
		}
		else if(ret_val > 1)
		{
			printf("\033[0;31mMinishell : command invalid \033[00m\n");
			free_all(token, len_token(cmd)); //-> IMPORTANT DE FOU, FIX DE LEAK
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
    // -> echo      (with option -n) parsing -> V token -> X
    // -> cd        (with only a relative or absolute path) parsing -> V token -> X
    // -> pwd       (with no options) parsing -> V token -> X
    // -> export    (with no options) parsing -> V token -> X
    // -> unset     (with no options) parsing -> X token -> X
    // -> env       (with no options or arguments) parsing -> X token -> X
    // -> exit      (with no options) parsing -> X token -> X
		

// !! TEMPORAL FIX, 11/11/2024 -> Add of a space at the end of the line in the case of 
// a command without space, because was blocking the correct execution of the pipe in certain case
