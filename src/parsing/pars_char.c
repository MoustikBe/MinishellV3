#include "../../minishell.h"

static char *get_bin_path(char *command, char *bin)
{
	char *bin_path;
	int i;
	int j;

	i = 0;
    j = 0;
	//printf("valeur de l'allocation de memoire -> %d\n", ft_strlen(command) + ft_strlen(bin) + 1);
	bin_path = malloc(sizeof(char) * ft_strlen(command) + ft_strlen(bin) + 1);
	bin_path[ft_strlen(command) + ft_strlen(bin)] = '\0';
	while(bin[i])
	{
		bin_path[i] = bin[i];
		i++;
	}
	while(command[j])
	{
		bin_path[i] = command[j];
		i++;
		j++;
	}
	bin_path[i] = '\0';
	return(bin_path);
}

int check_bin_quotes(char *command)
{
	int return_val;
	char *bin_path;

	if(command[0] == '\0')
		return(0);
	else if(str_cmp(command, "/") || str_cmp(command, ".") || str_cmp(command, ".."))
		return(0);
	bin_path = get_bin_path(command, "/usr/bin/");
	//printf("%d\n", open(bin_path, O_RDONLY));
	return_val = open(bin_path, O_RDONLY);
    if(return_val > 0)
	{
		free(bin_path); //-> Problème avec je ne sais pas pourquoi a debug dans le futur //
		close(return_val);
		return(1);
	}
	free(bin_path);
    return(0);
}
char *cmd_rebuild(char *cmd, char *cmd_check, int flag)
{
	char *cmd_rebuild;
	int len;
	int i;

	i = 0;
	len = 0;
	while (cmd[i] == ' ')
		i++;
	while(cmd[i])
	{
		if(cmd[i] == '"')
			i++;
		else
		{
			i++;
			len++;
		}
	}
	cmd_rebuild = malloc(sizeof(char) * len);
	i = 0;
	len = 0;
	while (cmd[i] == ' ')
		i++;
	while(cmd[i])
	{
		if(cmd[i] == '"')
			i++;
		else
		{
			cmd_rebuild[len] = cmd[i];
			i++;
			len++;
		}
	}
	cmd_rebuild[len] = '\0';
	return(cmd_rebuild);
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
	else if(check_bin_quotes(cmd_check) == 1)
		return(1);
	/*
	else if(check_syntax(cmd_check) == 1)
		return(8);
	else if(check_dolar(cmd_check) == 1)
		return(9);
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


