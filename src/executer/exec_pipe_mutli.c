#include "../../minishell.h"




void	step_1(pid_t pid, t_token *token, t_shell *shell)
{
	int i;
	int	fd_in;
	char *path;
	char *cmd_join;
	char **cmd_exec;

	if (pid == 0)
	{
		dup2(shell->fd[1], STDOUT_FILENO);
   		close(shell->fd[0]);
    	close(shell->fd[1]);
		// EXECUTION DE LA COMMANDE
		i = 0;
		cmd_join = calloc(1, 1);
		while(token[i].id != 6)
		{	
			cmd_join = ft_strjoin(cmd_join, token[i].str);
			cmd_join = ft_strjoin(cmd_join, " ");
			i++;
		}
		cmd_exec = ft_split(cmd_join, ' ');
		if(check_cmd_quotes(cmd_exec[0]) > 1)
		{
			//printf("BUILTIN DETECTED\n");
			if(check_cmd_quotes(cmd_exec[0]) == 2)
				echo(token, i);
			else if(check_cmd_quotes(cmd_exec[0]) == 3)
				cd(token[1].str);
			else if(check_cmd_quotes(cmd_exec[0]) == 4)
				pwd();
			exit(0);
		}
		path = make_path(cmd_exec[0]);
    	execve(path, cmd_exec, NULL);
	}
}

void	to_nb_cmd(t_shell *shell)
{
	close(shell->fd[1]);
	shell->fd_temp = shell->fd[0];
	pipe(shell->fd);
}

void pipe_mutli(t_token *token, t_shell *shell)
{
	int len;
	int i;
	char **split_cmd;
	pid_t pid;

	i = 0; 
	split_cmd = ft_split(shell->cmd, '|');
	while (split_cmd[i])
		i++;
	len = i - 2;
	pipe(shell->fd);
	pid = fork();
	if(pid == -1)
		printf("ERROR\n"); 
	step_1(pid, token, shell);
	to_nb_cmd(shell);
	/*
	WORK IN PROGRESS 
	sub_process(ppx, pid, argv, envp);
	from_nb_cmd(ppx);
	pid = fork();
	if (pid == -1)
		error();
	last_step(pid, ppx, argv, envp);
	wait_execution(argc - 3);
	close(ppx->fd_temp);
	STILL HAVE TO FINISH
	*/
}