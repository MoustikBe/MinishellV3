#include "../../minishell.h"
// ont a besoin du nombre de i dans l'array de struct 
void	echo(t_token *token, int j)
{

	/*	LES REGLES DU BULTIN ECHO */
	// ECHO peut recevoir n'importe quoi en parametre, 
	// premiere choses a faire est de voir si il y'a un flag au debut 
	// SI il y'a un flag appliquer le flag 
	// SI pas de flag continuer et ecrire les char 
	// Il est aussi possible que le token soit un infile ou outfile,
	// SI c'est le cas faire simplement le necessaire pour ne pas  
	// TRES BON POINT A NOTER !!!!!, echo <<test >out.txt n'est pas a prendre ne fonctionne pas sur bash 
	



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
