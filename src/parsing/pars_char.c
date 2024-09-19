#include "../../minishell.h"

char *cmd_rebuild(char *cmd, char *cmd_check, int flag)
{
	int len; 
	int i;
	int j;
	char *new_cmd;

	i = 0;
	len = ft_strlen(cmd_check);
	while (cmd[len])
	{
		len++;
		i++;
	}
	new_cmd = malloc(sizeof(char) * ft_strlen(cmd_check) + i - 1);
	i = 0;
	j = 0;
	while (cmd_check[i])
	{
		new_cmd[j] = cmd_check[i];
		j++;
		i++;
	}
	len = ft_strlen(cmd_check) + flag;
	while (cmd[len])
	{
		new_cmd[j] = cmd[len];
		len++;
		j++;
	}
	new_cmd[j] = '\0';
	return(new_cmd);
}


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
	int i;
	int len;
	int flag;
	int quotes;
	char *new_cmd;

	i = 0;
	len = 0;
	flag = 0;
	quotes = 0;
	while(shell->cmd[i] == ' ')
		i++;
	while(shell->cmd[i])
	{
		if(shell->cmd[i] == '"')
		{
			if(flag == 0 || flag == 2)
				flag = 1;
			else if(flag == 1)
				flag = 2;
			quotes++;
			i++;
		}
		else if(shell->cmd[i] == ' ' && flag == 1)
		{
			len++;
			i++;
		}
		else if(shell->cmd[i] == ' ' && flag == 0 || shell->cmd[i] == ' ' && flag == 2)
			break;
		else
		{ 
			len++;
			i++;
		}
	}
	new_cmd = malloc(sizeof(char) * len);
	i = 0;
	len = 0;
	while(shell->cmd[i])
	{
		if(shell->cmd[i] == '"')
		{
			if(flag == 0 || flag == 2)
				flag = 1;
			else if(flag == 1)
				flag = 2;
			i++;
		}
		else if(shell->cmd[i] == ' ' && flag == 1)
		{
			new_cmd[len] = shell->cmd[i];
			len++;
			i++;
		}
		else if(shell->cmd[i] == ' ' && flag == 0 || shell->cmd[i] == ' ' && flag == 2)
			break;
		else
		{
			new_cmd[len] = shell->cmd[i];
			len++;
			i++;
		}
	}
	new_cmd[len] = '\0';
	if(check_cmd_quotes(new_cmd) == 0)
	{
		free(new_cmd);
		return(0);
	}
	shell->cmd = cmd_rebuild(shell->cmd, new_cmd, flag);
	free(new_cmd);
	return(1);;
	// La on est caler sur le debut du mot qu'on va devoir copier

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