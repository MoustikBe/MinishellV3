#include "../../minishell.h"




void	step_1(pid_t pid, t_token *token, t_shell *shell, char *file_in)
{
	int i;
	int fd_in;
	char *path;
	char *cmd_join;
	char **cmd_exec;

	if (pid == 0)
	{
		if (file_in) 
		{
    	    fd_in = open(file_in, O_RDONLY);
    	    if (fd_in < 0) 
			{
    	        perror("open");
    	        exit(EXIT_FAILURE);
    	    }
    	    dup2(fd_in, STDIN_FILENO);
    	    close(fd_in);
    	}
    	// Duplication de la sortie du pipe vers stdout
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

void	command_execution(t_token *token, t_shell *shell)
{
	char *cmd_join;
	char *path;
	char **cmd_exec;

	cmd_join = calloc(1, 1);
	while(token[shell->index].id != 6)
	{	
		cmd_join = ft_strjoin(cmd_join, token[shell->index].str);
		cmd_join = ft_strjoin(cmd_join, " ");
		shell->index++;
	}
	cmd_exec = ft_split(cmd_join, ' ');
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


void createprocessus(t_shell *shell, t_token *token, pid_t pid)
{
    if (pid == 0)
    {
        close(shell->fd[0]);
        dup2(shell->fd_temp, STDIN_FILENO);
        dup2(shell->fd[1], STDOUT_FILENO);
        close(shell->fd_temp); // Fermez l'entrée temporaire après dup2
        close(shell->fd[1]);    // Fermez l'extrémité écriture après dup2

        command_execution(token, shell);
        exit(0);
    }
    close(shell->fd[1]); // Fermez l'extrémité écriture dans le processus parent
    shell->fd_temp = shell->fd[0];
    if (shell->nb_cmd > 1) {
        pipe(shell->fd);
    }
    shell->nb_cmd--;
}

void	sub_process(t_shell *shell, t_token *token, pid_t pid)
{
	fprintf(stderr ,"shell->nb_cmd -> %d\n", shell->nb_cmd);
	while (shell->nb_cmd)
	{
		pid = fork();
		if (pid == -1)
			exit(2);
		fprintf(stderr, "shell->index -> %d\n token -> %s\n", shell->index, token[shell->index].str);
		if(token[shell->index].id == 6)
			shell->index++;
		createprocessus(shell, token, pid);
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
        dup2(shell->fd[0], STDIN_FILENO);
        close(shell->fd[1]);
        close(shell->fd[0]); // Fermez après duplication pour éviter les fuites

        command_execution(token, shell);
        exit(0);
    }
    close(shell->fd[0]); // Fermez l'extrémité lecture dans le parent
}

void wait_execution(int nb_cmd)
{
	int status;

	while(nb_cmd--)
		waitpid(-1, &status, 0);
}


void	to_nb_cmd(t_shell *shell)
{
	close(shell->fd[1]);
	shell->fd_temp = shell->fd[0];
	pipe(shell->fd);
}

void pipex_multi(t_token *token, t_shell *shell)
{
	int len;
	int i;
	char *file_in;
	char *file_out;
	char **split_cmd;
	pid_t pid;

	file_in = NULL;
	file_out = NULL;
	i = 0;
	while(token[i].str)
	{
		if(token[i].id == 5)
			file_in = ft_strdup(token[i].str);
		else if(token[i].id == 4)
			file_out = ft_strdup(token[i].str);
		i++;
	}
	i = 0;
	split_cmd = ft_split(shell->cmd, '|');
	while (split_cmd[i])
		i++;
	shell->nb_cmd = i - 2;

	pipe(shell->fd);
	pid = fork();
	if(pid == -1)
		printf("ERROR\n"); 

	/* Work in progress */
	
	// SAVOIR GERER PLUSIEURS INFILE ET OUTFILE ? COMMENT ? 
	// Pour commencer faison en sorte de gerer des infiles et outfile. 
	// Seulement si il se trouve dans la premiere ou derniere etape. 
	// COPIE de la facon de faire dans le simple pipe. 
	step_1(pid, token, shell, file_in);
	to_nb_cmd(shell);
	//fprintf(stderr, "After_pipe_mngt\n");
	sub_process(shell, token, pid);
    from_nb_cmd(shell);
	//fprintf(stderr, "Before last step\n");
    pid = fork();
    if (pid == -1)
        exit(2);

    last_step(shell, token, pid);
    wait_execution(shell->nb_cmd + 2);
    close(shell->fd_temp); 
}
