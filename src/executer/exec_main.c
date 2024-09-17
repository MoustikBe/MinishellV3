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


void exec_bin(t_token *token, char *cmd, char **envp)
{
	char *path;
	char **command;

	path = make_path(token[0].str);
	command = ft_split(cmd, ' ');
	execve(path, command, envp);

}

void exec_main(t_token *token, char *cmd, char **envp, t_shell *shell)
{
	pid_t pid;
	//printf("%d\n", shell->len_token);
	if(token[0].id == 10)
	{
		pid = fork();
		if(pid == 0)
			exec_bin(token, cmd, envp);
		else
			wait(0);
	}
	// Echo
	//else if(token[0].id == 11)
	//{

	//}
	return ;
}

/*
char **envp
char **argv

argv[2] = "ls -la"
split par 32 ls -l! arg[0] 
				-la -s -s arg + 1 
execve(arg[0], envp, arg + 1)

append-> malloc(sizeof(len de mon string avec les quotes))
join

"echo "'123455'""|  ls -la


double mode (*len_token)
5

single mode (*len_token):
5 + 6

token = malloc (*len_token - start);
copy(line depuis start a len token dans token)

*/

