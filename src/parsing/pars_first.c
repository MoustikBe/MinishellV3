#include "../../minishell.h"

/* Verification of the first element of the command */
int first_element(char *cmd)
{

	if(str_cmp(cmd, "echo") == 1)
		return(2);
	else if(str_cmp(cmd, "cd") == 1)
		return(3);
	else if(str_cmp(cmd, "pwd") == 1)
		return(4);
	else if(str_cmp(cmd, "export") == 1)
		return(5);
	else if(str_cmp(cmd, "unset") == 1)
		return(6);
	else if(str_cmp(cmd, "env") == 1)
		return(7);
	else if(str_cmp(cmd, "exit") == 1)
		return(8);
	else if(check_syntax(cmd) == 1)
		return(9);
	else if(check_dolar(cmd) == 1)
		return(10);
	else if(check_bin(cmd) == 1)
		return(1);
	return(0);
}

