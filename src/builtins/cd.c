#include "../../minishell.h"

void cd(char *path)
{
	int return_val;
	return_val = chdir(path);
	if(return_val < 0)
		printf("ERROR :/ \n");
	return;

}

// test // 