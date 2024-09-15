#include "minishell.h"

int main(int argc, char **argv, char **envp)
{
    char *cmd;
    t_token *token;
    int ret_val;

    //token = malloc(sizeof(t_token));
    while (1)
    {
        cmd = readline("\033[34;01mMinishell : \033[00m");
		if(cmd[0] == '\0')
			ret_val = 0;
		else 
			ret_val = parsing_main(cmd); //parsing
        if(ret_val == 0)
		{
            printf("\033[0;31mMinishell : command not found -> %s\033[00m\n", cmd);
        	//break ; // Quand je test mes leaks //
		}
		else if(ret_val == 1)
		{
			// PARSING VALIDER, la commande peut ce faire tokeniser, 
			// AVANT il faut simplement être sure que toute la memoire allouer a été correctement liberer // 
			token = token_main(cmd, token);
			//break;
			exec_main(token, cmd, envp);
		}
		else if(ret_val > 1)
		{
			printf("\033[0;31mMinishell : command invalid \033[00m\n");
        	//break ; // Quand je test mes leaks //
		}
        add_history(cmd);
        free(cmd);
    }
    free(cmd);
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