#include "../../minishell.h"

void token_parser(t_token *token, t_shell *shell, int len)
{
	int i;

	i = 0;
	while(i < len)
	{
		// Vérifie le cas ou il y'a deux pipe côte a côte // 
		if(i + 1 < len)
		{
			if(token[i].id == 6 && token[i + 1].id < 3)
				shell->error = 1;
			if(token[i].id == 6 && token[i + 1].id == 6)
				shell->error = 1;
		}
		i++;
	}
	// Parcourir toute l'array de token, checker si l'id du pipe n'est pas répèter
	return ; // -> in case of error
}
