#include "../../minishell.h"
// ont a besoin du nombre de i dans l'array de struct 
void	echo(t_token *token, int j)
{
	int i;

	// Echo is identify now we go direct to the second part of the commmand
	i = 1;
	while(i < j && token[i].id != 6)
	{
		printf("%s ", token[i].str);
		i++;
	}
	printf("\n");
}
