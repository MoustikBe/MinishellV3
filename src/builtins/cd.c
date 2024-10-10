#include "../../minishell.h"

void cd(t_token *token)
{
	int return_val;
	return_val = chdir(token[1].str);
	if(return_val < 0)
		printf("ERROR :/ \n");
	return;

}

// test // 