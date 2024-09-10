#include "../../minishell.h"


void token_copy(t_token *token, char *cmd, int i)
{
	int flag;
	int j;
	int k;

	k = 0;
	j = 0;
	flag = 0;
	// echo test      0     1 
	while(cmd[j] && flag < i)
	{
		if(cmd[j] == ' ')
			flag++;
		j++;
	}
	while(cmd[j] == ' ')
		j++;
	while(cmd[j] && cmd[j] != ' ')
	{
		token[i].str[k] = cmd[j];
		k++;
		j++;
	}
	token[i].str[k] = '\0';
}