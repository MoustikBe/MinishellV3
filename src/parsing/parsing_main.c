#include "../../minishell.h"


int parsing_main(t_shell *shell)
{
	// Savoir si il s'agit d'une commande écrite au début ou alors ou <, > ou <<, >> et $VARIABLE. Seul ses
	char **pars_cmd;
	int ret_val;


	if(quotes_mod(shell) == 0)
		return(0);
	return(1);
	pars_cmd = ft_split(shell->cmd, ' ');
	ret_val = first_element(pars_cmd[0]); 
	// Le parsing est seulement extremement important dans le cas des builtins. //
	// Dans le cas d'un chemin binaire le parsing s'execute avec le excve //
    if(ret_val == 1)
	{
		// /bin/cmd -> pas besoin de + de parsing // 
		free_array(pars_cmd);
		return(1);
	}
	else if(ret_val > 1)
    {
		// Verifier les autres element // 
		// echo -> pas besoin de parsing //
		// pwd -> pas besoin de parsing // 
		// unset -> pas besoin de parsing // 
		// env -> pas besoin de parsing // 

		/*  -- Parsing de cd -- */
		if(ret_val == 3)
		{	
			if(pars_cd(pars_cmd) == 0)
			{
				free_array(pars_cmd);
				return(2);
			}
			free_array(pars_cmd);
			return(1);
		}
		/*  -- Parsing d'export -- */
		if(ret_val == 5)
		{	
			if(pars_export(pars_cmd) == 0)
			{
				free_array(pars_cmd);
				return(2);
			}
			free_array(pars_cmd);
			return(1);
		}
		/*  -- Parsing de syntax -- */
		if(ret_val == 8)
		{	
			if(pars_syntax(pars_cmd) == 0)
			{
				free_array(pars_cmd);
				return(2);
			}
			free_array(pars_cmd);
			return(1);
		}
		/*  -- Parsing de dollar -- */
		if(ret_val == 9)
		{	
			if(pars_dolar(pars_cmd) == 0)
			{
				free_array(pars_cmd);
				return(2);
			}
			free_array(pars_cmd);
			return(1);
		}
		// SI tout est bon, pas oublier de free et de return(1);
		free_array(pars_cmd);
		return(1);
    }
	
    // Verif_cmd V Verif_input X Verif_$ X
	// 0 = parsing non valide //
	free_array(pars_cmd);
	return(0);
}
