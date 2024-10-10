#include "../../minishell.h"

// -- Work in progress -- // 

void child_process(int fd[2], char *child_cmd, char *file_in) 
{
    int fd_in;
	char **cmd_exec;
	char *path;

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
    dup2(fd[1], STDOUT_FILENO);
    close(fd[0]);
    close(fd[1]);
	cmd_exec = ft_split(child_cmd, ' ');
	path = make_path(cmd_exec[0]);
    // Exécution de la commande
    execve(path, cmd_exec, NULL);
}


// Processus parent pour la deuxième commande
void parent_process(int fd[2], char *parent_cmd, char *file_out) 
{
    int fileout;
	char **cmd_exec;
	char *path;

    wait(0);
    // Duplication de l'entrée du pipe vers stdin
    dup2(fd[0], STDIN_FILENO);
    close(fd[1]);
    close(fd[0]);
    if (file_out) {
        fileout = open(file_out, O_WRONLY | O_CREAT | O_TRUNC, 0777);
        if (fileout == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        dup2(fileout, STDOUT_FILENO);
        close(fileout);
    }
	cmd_exec = ft_split(parent_cmd, ' ');
	path = make_path(cmd_exec[0]);
    // Exécution de la commande
    execve(path, cmd_exec, NULL);
}


void pipex_simple(t_token *token, t_shell *shell)
{
	// il y'a que un pipe -> echo test | pwd  
	// On fait un split avec | comme delimiter. 
	// Apres il suffit de refaire un split  sur le split si la commande est dans le  /bin
	// Ou d'executer le builtin 
	char **cmd_split;
	char *fd_in;
	char *fd_out;
	pid_t pid;
	int fd[2];
	int i;

	fd_in = NULL;
	fd_out = NULL;
	i = 0;
	while(token[i].str)
	{
		// Problem ici avec le nom il faut tej > et <  avant le nom
		if(token[i].id == 5)
			fd_in = clean_name(token[i].str);
		else if(token[i].id == 4)
			fd_out = clean_name(token[i].str);
		i++;
	}

	pipe(fd);
	cmd_split = ft_split(shell->cmd, '|');
	pid = fork();
	if(pid == -1)
		return ; // ERREUR 
	else if(pid == 0)
		child_process(fd, cmd_split[0], fd_in);
	else
		parent_process(fd, cmd_split[1], fd_out);
		// Parent process	
	

	// la on sait que il y'a que un pipe // 

}
// -- Work in progress -- // 