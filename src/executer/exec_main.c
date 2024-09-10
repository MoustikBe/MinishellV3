#include "../../minishell.h"


char *make_path(char *token)
{
	char *bin = "/usr/bin/";
	char *path;
	int i = 0;
	int j = 0;

	path = malloc(sizeof(char) * ft_strlen(bin) + ft_strlen(token));
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
	return(path);
}


void exec_bin(t_token *token, char *cmd)
{
	char *path;
	char **command;

	path = make_path(token[0].str);
	printf("%s\n", path);
	//command = ft_split(cmd, ' ');
	//execve(path, command, NULL);

}



void exec_main(t_token *token, char *cmd)
{
	printf("%d\n",token[0].id);
	//if(token[0].id == 10)
	//	exec_bin(token, cmd);
	//return ;
}