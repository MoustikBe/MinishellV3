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
	printf("space -> %d\n", space);
	return(space);

}


// Toute la memoire a été correctement liberer et une commande valide a été trouver // 
int token_main(char *cmd, t_token *token)
{
	printf("len_space -> %d\n", len_token(cmd));
	//token = malloc(sizeof(t_token) * len_token(cmd));

	return(1);
}