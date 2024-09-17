#include "../../minishell.h"

int check_cmd_quotes(char *cmd_check)
{
	if(str_cmp_quotes(cmd_check, "echo") == 1)
		return(1);
	else if(str_cmp_quotes(cmd_check, "cd") == 1)
		return(1);
	else if(str_cmp_quotes(cmd_check, "pwd") == 1)
		return(1);
	else if(str_cmp_quotes(cmd_check, "export") == 1)
		return(1);
	else if(str_cmp_quotes(cmd_check, "unset") == 1)
		return(1);
	else if(str_cmp_quotes(cmd_check, "env") == 1)
		return(1);
	/*
	else if(check_syntax(cmd_check) == 1)
		return(8);
	else if(check_dolar(cmd_check) == 1)
		return(9);
	else if(check_bin(cmd_check) == 1)
		return(1);
	else if(check_char(cmd_check) == 1)
		return(10);
	*/
	else if(str_cmp_quotes(cmd_check, "exit") == 1)
		return(1);
	return(0);
}

int quotes_mod(t_shell *shell)
{
	char *cmd_check;
	int i;
	int j;
	int flag;

	i = 0;
	flag = 0;
	while (shell->cmd[i])
	{
		if(shell->cmd[i] == '"')
			flag++;
		i++;
	}
	// Check si il y'a bien un nb pair de quotes
	if(flag % 2 != 0)
		return (0);
	i = 0;
	j = 0;
	// Prendre toute la partie entre les deux quotes, allocation
	while(shell->cmd[i])
	{
		if(shell->cmd[i] == ' ')
			j = 0;
		if(shell->cmd[i] == '"')
		{
			i++;
			while (shell->cmd[i] != '"')
			{
				i++;
				j++;
			}
			while(shell->cmd[i] != ' ')
			{
				i++;
				j++;
			}
			break ;
		}
		else
			i++;
	}
	cmd_check = malloc(sizeof(char) * j);
	// Copy
	i = 0;
	j = 0;
	while(shell->cmd[i])
	{
		if(shell->cmd[i] == '"')
		{
			i++;
			while (shell->cmd[i] != '"')
			{
				cmd_check[j] = shell->cmd[i];
				j++;
				i++;
			}
			break;
		}
		else
			i++;
	}
	cmd_check[j] = '\0';
	// Check la commande 
	if(check_cmd_quotes(cmd_check) == 0)
		return(0);
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
	else if(str_cmp(cmd, "exit") == 1)
		return(1);
	else if(check_bin(cmd) == 1)
		return(1);
	return(0);	
}