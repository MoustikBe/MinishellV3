#include "../../minishell.h"

static int check_char(char *cmd)
{
	int i;

	i = 0;
	if(cmd[i] == '"')
	{
		i++;
		while(cmd[i] != '"' && cmd[i] != '\0')
			i++;
	}
	//printf("len -> %d\n", ft_strlen(cmd));
	//printf("i -> %d\n", i);
	if(ft_strlen(cmd) - 1 == i)
		return(1);
	return(0);
}

static int check_syntax(char *cmd)
{
	if(cmd[0])
	{
		if(cmd[0] == '<' || cmd[0] == '>')
			return(1);
	}
	return(0);

}

static int check_dolar(char *cmd)
{
	int i;

	i = 0;
	if(cmd[i] == '$')
		i++;
	else 
		return(0);
	while(((cmd[i] >= 48) && (cmd[i] <= 57)) || ((cmd[i] >= 65) && (cmd[i] <= 90)) || ((cmd[i] >= 97) && (cmd[i] <= 122)) || cmd[i] == '_')
		i++;
	if(i == ft_strlen(cmd))
		return(1);
	return(0);
}

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
	while(command[j] && command[j] != ' ')
	{
		bin_path[i] = command[j];
		i++;
		j++;
	}
	bin_path[i] = '\0';
	return(bin_path);
}

int check_bin(char *command)
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
	else if(check_syntax(cmd) == 1)
		return(8);
	else if(check_dolar(cmd) == 1)
		return(9);
	else if(check_bin(cmd) == 1)
		return(1);
	else if(check_char(cmd) == 1)
		return(10);
	else if(str_cmp(cmd, "exit") == 1)
		return(11);
	return(0);
}

