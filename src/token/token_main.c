#include "../../minishell.h"

static int len_token(char *cmd)
{
	int space; 
	int i;

	space = 0;
	i = 0;
	while(cmd[i])
	{
		if(cmd[i] == ' ')
		{	
			while(cmd[i] == ' ')
				i++;
			space++;
		}
		else
			i++;
	}
	return(space + 1);

}

static int len_for_token(char *cmd, int i)
{
	int j = 0;
	while(cmd[i] == ' ')
		i++;
	while(cmd[i] && cmd[i] != ' ')
	{
		i++;
		j++;
	}
	return(j);
}

static void free_all(t_token *token, int len)
{
	int i;

	i = 0;
	while(i < len)
	{
		free(token[i].str);
		i++;
	}
	free(token);

}

// Toute la memoire a été correctement liberer et une commande valide a été trouver // 
t_token *token_main(char *cmd, t_token *token)
{
	int i;
	int j;
	int len_space;

	i = 0;
	j = 0;
	token = malloc(sizeof(t_token) * len_token(cmd));
//	if(!token)
//		return(0);
	while(i < len_token(cmd))
	{
		token[i].id = 0;
		len_space = len_for_token(cmd, j);
		// alloc de la memoire pour le char de la strucute // 
		token[i].str = malloc(sizeof(char) * len_space + 1);
//		if(!token[i].str)
//			return(0);
		token_copy(token, cmd, i);
		//printf("len_for_token -> %d\n", len_space);
		//printf("\033[0;31mtoken[i].str -> %s\033[00m\n", token[i].str);
		token_id(token, i);
		//printf("\033[0;33mtoken[i].id  -> %d\033[00m\n", token[i].id);
		j = j + len_space + 1;
		i++;
	}
	//free_all(token, len_token(cmd));
	return(token);
}