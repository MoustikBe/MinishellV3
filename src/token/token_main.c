#include "../../minishell.h"



// Toute la memoire a été correctement liberer et une commande valide a été trouver // 
t_token *token_main(char *cmd, t_token *token, t_shell *shell)
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
	shell->len_token = len_token(cmd);
	//free_all(token, len_token(cmd)); //-> IMPORTANT DE FOU, FIX DE LEAK
	return(token);
} 