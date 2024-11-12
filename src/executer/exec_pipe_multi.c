#include "../../minishell.h"


void	step_1(pid_t pid, t_token *token, t_shell *shell)
{
	int	fd_in;
	char *path;
	char *cmd_join;
	char **cmd_exec;
	if (pid == 0)
	{
		//fprintf(stderr,"TEST_STEP_1\n");
		dup2(shell->fd[1], STDOUT_FILENO);
   		close(shell->fd[0]);
    	close(shell->fd[1]);
		// EXECUTION DE LA COMMANDE
		shell->index = 0;
		cmd_join = calloc(1, 1);
		while(token[shell->index].id != 6)
		{	
			cmd_join = ft_strjoin(cmd_join, token[shell->index].str);
			cmd_join = ft_strjoin(cmd_join, " ");
			shell->index++;
		}
		cmd_exec = ft_split(cmd_join, ' ');
		//fprintf(stderr, "step 1 | cmd_exec -> %s\n", cmd_exec[0]);
		if(check_cmd_quotes(cmd_exec[0]) > 1)
		{
			//printf("BUILTIN DETECTED\n");
			if(check_cmd_quotes(cmd_exec[0]) == 2)
				echo(token, shell->index);
			else if(check_cmd_quotes(cmd_exec[0]) == 3)
				cd(token[1].str);
			else if(check_cmd_quotes(cmd_exec[0]) == 4)
				pwd();
			exit(0);
		}
		path = make_path(cmd_exec[0]);
		//fprintf(stderr, "step 1 | path -> %s\n", path);
    	execve(path, cmd_exec, NULL);
	}
}

void	to_nb_cmd(t_shell *shell)
{
	close(shell->fd[1]);
	shell->fd_temp = shell->fd[0];
	pipe(shell->fd);
}

void	command_execution(t_token *token, t_shell *shell)
{
	char *cmd_join;
	char *path;
	char **cmd_exec;

	cmd_join = calloc(1, 1);
	while(token[shell->index].id != 6 && token[shell->index].id)
	{	
		cmd_join = ft_strjoin(cmd_join, token[shell->index].str);
		cmd_join = ft_strjoin(cmd_join, " ");
		shell->index++;
	}
	cmd_exec = ft_split(cmd_join, ' ');
	//fprintf(stderr, "mid step | cmd_exec -> %s\n", cmd_exec[0]);
	if(check_cmd_quotes(cmd_exec[0]) > 1)
	{
		//printf("BUILTIN DETECTED\n");
		if(check_cmd_quotes(cmd_exec[0]) == 2)
			echo(token, shell->index);
		else if(check_cmd_quotes(cmd_exec[0]) == 3)
			cd(token[1].str);
		else if(check_cmd_quotes(cmd_exec[0]) == 4)
			pwd();
		exit(0);
	}
	path = make_path(cmd_exec[0]);
	execve(path, cmd_exec, NULL);
	// On pourrait donner un index dans la structure, 
	// Une fois a cette index, on dit que jusque au prochain pipe on avance et join les mots 
	// Donc avec sa on arrive a la methode de d'habitude et apres on update l'index.
}


void	createprocessus(pid_t pid, t_shell *shell, t_token *token)
{
	if (pid == 0)
	{
		close(shell->fd[0]);
		dup2(shell->fd_temp, STDIN_FILENO);
		dup2(shell->fd[1], STDOUT_FILENO);
		close(shell->fd_temp);
		close(shell->fd[1]);
		command_execution(token, shell);
	}
	if (shell->nb_cmd > 1)
	{
		close(shell->fd_temp);
		close(shell->fd[1]);
		shell->fd_temp = shell->fd[0];
		pipe(shell->fd);
	}
	shell->nb_cmd--;
	//fprintf(stderr, "shell.index %d | token.str -> %s\n", shell->index, token[shell->index].str);
	while (token[shell->index].id != 6)
		shell->index++;
	shell->index++;
}

void	sub_process(t_shell *shell, t_token *token, pid_t pid)
{
	if(shell->index == 0)
	{
		while(token[shell->index].id != 6)
			shell->index++;
	}
	shell->index++;
	while (shell->nb_cmd)
	{
		pid = fork();
		if (pid == -1)
			exit(2);
		if(token[shell->index].id == 6)
			shell->index++;
		createprocessus(pid, shell, token);
	}
}

void	from_nb_cmd(t_shell *shell)
{
	close(shell->fd_temp);
	close(shell->fd[1]);
	shell->fd_temp = shell->fd[0];
}

void last_step(t_shell *shell, t_token *token, pid_t pid)
{
    if (pid == 0)
    {
		fprintf(stderr, "STARFOULA_3\n");
        dup2(shell->fd[0], STDIN_FILENO);
        close(shell->fd[1]);
        close(shell->fd[0]); // Fermez après duplication pour éviter les fuites
		fprintf(stderr, "STARFOULA_4\n");
        command_execution(token, shell);
        exit(0);
    }
	fprintf(stderr, "STARFOULA_5\n");
    close(shell->fd[0]); // Fermez l'extrémité lecture dans le parent
}

void wait_execution(int nb_cmd)
{
	int status;

	while(nb_cmd--)
	{
		//fprintf(stderr , "waiting\n");		
		waitpid(-1, &status, 0);
	}
}

void pipex_multi(t_token *token, t_shell *shell)
{
    int i;
    char **split_cmd;
    pid_t pid;

	shell->index = 0;
    i = 0;
    split_cmd = ft_split(shell->cmd, '|');
    while (split_cmd[i])
        i++;
    shell->nb_cmd = i - 2;
    pipe(shell->fd);
    pid = fork();
    if (pid == -1)
        printf("ERROR\n");

    step_1(pid, token, shell);
	fprintf(stderr, "step1 -> shell.index %d | token.str -> %s\n", shell->index, token[shell->index].str);
	close(shell->fd[1]);  // Fermez l'extrémité écriture après step_1 dans le parent
    to_nb_cmd(shell);
    sub_process(shell, token, pid);
	fprintf(stderr, "sub_process -> shell.index %d | token.str -> %s\n", shell->index, token[shell->index].str);
    from_nb_cmd(shell);
	fprintf(stderr, "STARFOULA_1\n");
    pid = fork();
    if (pid == -1)
        exit(2);
	fprintf(stderr, "STARFOULA_2\n");
    last_step(shell, token, pid);
    wait_execution(shell->nb_cmd + 3);
    close(shell->fd_temp);  // Fermez l'extrémité temporaire
	return ;
}




/*
void	last_step(pid_t pid, t_shell *shell, t_token *token)
{
	if (pid == 0)
	{
		dup2(shell->fd_temp, STDIN_FILENO);
		close(shell->fd_temp);
		command_execution(token, shell);
	}
	close(shell->fd[0]);
}
*/