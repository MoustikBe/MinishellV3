#include "../../minishell.h"
// ont a besoin du nombre de i dans l'array de struct 
void	echo(t_token *token, int j)
{


	// Echo is identify now we go direct to the second part of the commmand
	j++;
	//fprintf(stderr, "j -> %d\n", j);
	while(token[j].id != 6 && token[j].str)
	{
		printf("%s ", token[j].str);
		j++;
	}
	printf("\n");
}
