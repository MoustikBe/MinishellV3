#include "../../minishell.h"


char *make_path(char *token)
{
	char *bin = "/usr/bin/";
	char *path;
	int i = 0;
	int j = 0;

	path = malloc(sizeof(char) * ft_strlen(bin) + ft_strlen(token) + 1);
	if(!path)
		return(NULL);
	while(bin[i])
	{
		path[j] = bin[i];
		i++;
		j++;
	}
	i = 0;
	while(token[i])
	{
		path[j] = token[i];
		i++;
		j++;
	}
	path[j] = '\0';
	return(path);
}


void exec_bin(t_token *token, char *cmd)
{
	char *path;
	char **command;

	path = make_path(token[0].str);
	command = ft_split(cmd, ' ');
	execve(path, command, NULL);

}



void exec_main(t_token *token, char *cmd)
{
	pid_t pid;
	//printf("%d\n",token[0].id);
	if(token[0].id == 10)
	{
		pid = fork();
		if(pid == 0)
			exec_bin(token, cmd);
		else
			wait(0);
	}
	return ;
}