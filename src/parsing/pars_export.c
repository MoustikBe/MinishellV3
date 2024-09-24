#include "../../minishell.h"

int pars_export(char **pars_cmd)
{
	int i;
	int j;

	i = 1;
	while(pars_cmd[i])
	{
		j = 0;
		while(pars_cmd[i][j] != '=' && pars_cmd[i][j] != '\0')
		{
			if(ft_isalnum(pars_cmd[i][j]) == 0 && pars_cmd[i][j] != '|' || pars_cmd[i][j] == '|' && pars_cmd[i][j - 1] == '|')
			{	
				printf("\033[0;31mNot a valid identifier \033[00m");
				return(0);
			}
			j++;
		}
		i++;
	}
	return(1);
}
