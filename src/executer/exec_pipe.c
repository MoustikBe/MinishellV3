#include "../../minishell.h"

// -- Work in progress -- // 

char **make_command(t_token *token, int i)
{
    char **command;
    int count = 0;
    int i_s = i;

    while(token[i_s].str && token[i_s].id != 6)
    {
        i_s--;
        count++;
    }    
    command = malloc(sizeof(char) * count + 1);
	printf("count -> %d\n", count);
    count = 0;
    while(token[i].str && token[i].id != 6)
    {
        command[count] = ft_strdup(token[i].str);
        i--;
        count++;
    }
	command[count] = NULL;
	i = 0;
	while(command[i])
	{
		printf("array -> %s\n", command[i]);
		i++;
	}
    return(command);
}

void pipex(t_token *token)
{
	int i; 
	char **command;

	i = 0;
	while(token[i].str)
	{
    	while(token[i].id != 6) 
			i++;
		i++;
		command = make_command(token, i);
	}
}
// -- Work in progress -- // 