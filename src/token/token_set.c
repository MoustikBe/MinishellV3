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

void token_id(t_token *token, int i)
{
	if(is_cmd(token, i) == 1)
		return ;
	else if(is_flag(token, i) == 1)
		return ;
	else if(is_outfile(token, i) == 1)
		return ;
	else if(is_infile(token, i) == 1)
		return ;
	else if(is_pipe(token, i) == 1)
		return ;
	else if(is_char(token, i) == 1)
		return ;
	return ;
}