#include "../../minishell.h"


char *make_path(char *token)
{
	char *bin = "/usr/bin/";
	char *path;
	int i = 0;
	int j = 0;

	if(direct_bin(token))
		return(token);
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
	int i;

	path = make_path(token[0].str);
	command = ft_split(cmd, ' ');
	i = 0;
	while(command[i])
	{
		// Problem ici avec le nom il faut tej > et <  avant le nom
		command[i] = clean_name(command[i]);
		i++;
	}
	execve(path, command, envp);

}

int check_pipe(t_token *token)
{
	int i;
	int count;

	i = 0;
	count = 0;
	while(token[i].str)
	{
		if(token[i].id == 6)
			count++;
		i++;
	}
	if(count == 1)
		return(1);
	else if(count > 1)
		return(2);
	return(0);
}


void exec_main(t_token *token, char *cmd, char **envp, t_shell *shell)
{
	pid_t pid;
	// FIRST check if there is a pipe in all the command //
	// Pipe -> autre direction d'execution // 
	if(check_pipe(token) == 1)
	{
		pid = fork();
		if(pid == 0)
			pipex_simple(token, shell);
		else
			wait(0);
	}
	// AVOIR FINIS 100% du normal avant de faire celui la. 
	else if(check_pipe(token) == 2)
		pipex_multi(token, shell);
	
	else 
	{
		// Execution d'une commande // 
		if(token[0].id == 10)
		{
			pid = fork();
			if(pid == 0)
				exec_bin(token, cmd, envp);
			else
				wait(0);
		}
		// Echo
		else if(token[0].id == 11)
			echo(token, shell->len_token);
		else if(token[0].id == 12)
			cd(token[1].str);
		else if(token[0].id == 13)
			pwd();
	}
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

