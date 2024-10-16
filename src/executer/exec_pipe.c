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
	// FAIRE une verif, soit il s'agit d'un builtin soit il s'agit 
	// d'une commande binaire faire la diff avnt de continuer.
	//if(is_buitlin(cmd_exec[0]) == 1) // SI il y'a un resultat exec le builtin a la place
	//{

	//}
	// FAIRE UNE VERIF AVANT L'EXEC REGLE LE PROB  DE <main.c | pwd
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
	int id_exec;
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
	// Le probleme vient du faite que je fais un split du shell->cmd et pas de token
	cmd_split = ft_split_basic(shell->cmd, '|');
	i = 0;
	while(cmd_split[i])
	{
		// Problem ici avec le nom il faut tej > et <  avant le nom
		cmd_split[i] = clean_name(cmd_split[i]);
		i++;
	}
	pid = fork();
	if(pid == -1)
		return ; // ERREUR
	// AJOUTER UN CODE D'EXECUTION quand on execute la commande child et parent
	// afin de differencier les builtins des /bin
	
	else if(pid == 0)
		child_process(fd, cmd_split[0], fd_in);
	else
		parent_process(fd, cmd_split[1], fd_out);
		// Parent process	
	
	// Petit probleme d'execution avec le quotes, quand on fait par exemple
	// -> cat main.c | grep "dwadwwad" Il croit que dwadwwad est une commande a cause de quotes mod
	// POSSIBLE SOLUTION, utiliser le split de base et pas le split modifier juste pour cette partie de code
	// FIX, mais maintenant il faut encore FIX le probleme de l'execution ne soit pas une execution sur le binaire 
	// Mais que si il y'a une execution de echo ou autre builtin que sa soit l'execution de notre echo qui soit executer

	// la on sait que il y'a que un pipe // 

}
// -- Work in progress -- // 