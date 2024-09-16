#include "../../minishell.h"

int quotes_mod(t_shell *shell)
{
	char *cmd_check;
	int i;
	int j;

	i = 0;
	j = 0;
	while(shell->cmd[i] != '\0')
	{	
		if(shell->cmd[i] != '"')
		{
			i++;
			j++;
		}
		else
			i++;
	}
	cmd_check = malloc(sizeof(char) * j + 1);
	i = 0;
	j = 0;
	while (shell->cmd[i] != '\0')
	{
		if(shell->cmd[i] != '"')
		{
			cmd_check[j] = shell->cmd[i];
			j++;
			i++; 
		}
		else
			i++;
	}
	cmd_check[j] = '\0';
	shell->cmd = ft_strdup(cmd_check);
	if(try_char(cmd_check) == 0)
	{
		free(cmd_check);
		return(0);
	}
	free(cmd_check);
	return(1);
}


int try_char(char *cmd)
{
	if(str_cmp(cmd, "echo") == 1)
		return(1);
	else if(str_cmp(cmd, "cd") == 1)
		return(1);
	else if(str_cmp(cmd, "pwd") == 1)
		return(1);
	else if(str_cmp(cmd, "export") == 1)
		return(1);
	else if(str_cmp(cmd, "unset") == 1)
		return(1);
	else if(str_cmp(cmd, "env") == 1)
		return(1);
	else if(check_bin(cmd) == 1)
		return(1);
	return(0);	
}