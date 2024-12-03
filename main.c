#include "minishell.h"





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
		if(shell->cmd[0] == '\0' || verif_quotes(shell->cmd))
		{
			cpy_cmd = ft_strdup(shell->cmd);
			ret_val = 0;
		}
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
            printf("\033[0;31mMinishell : command invalid \033[00m\n");
			shell->last_exit_status = 127;
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
			{
				printf("\033[0;31mMinishell : command invalid \033[00m\n");
				shell->last_exit_status = 127;
			}
			else
				exec_main(token, shell->cmd, envp, shell);
			//if(shell->error == 1)
			//	printf("\033[0;31mMinishell : command invalid \033[00m\n");
			//free_all_token(token, len_token(cmd)); //-> IMPORTANT DE FOU, FIX DE LEAK
			//break ;
		}
		else if(ret_val > 1)
		{
			printf("\033[0;31mMinishell : command invalid \033[00m\n");
			shell->last_exit_status = 127;
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


