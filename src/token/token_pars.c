#include "../../minishell.h"
/*
static int menu_token_verif(t_token *token, int i)
{
	// RETURN 0 == probleme
	if(token[i].id == 5)
	{
		// NEED TO DO A CUSTOM FUNCTION FOR SUB_PARSING //
	}
}
*/

static int quotes_mod_bis(char *to_pars)
{
	int i;
	int len;
	int flag;
	int quotes;
	char *new_cmd;

	i = 0;
	len = 0;
	flag = 0;
	quotes = 0;
	while(to_pars[i] == ' ')
		i++;
	while(to_pars[i])
	{
		if(to_pars[i] == '"')
		{
			if(flag == 0 || flag == 2)
				flag = 1;
			else if(flag == 1)
				flag = 2;
			quotes++;
			i++;
		}
		else if(to_pars[i] == ' ' && flag == 1)
		{
			len++;
			i++;
		}
		else if(to_pars[i] == ' ' && flag == 0 || to_pars[i] == ' ' && flag == 2)
			break;
		else
		{ 
			len++;
			i++;
		}
	}
	new_cmd = malloc(sizeof(char) * len);
	i = 0;
	len = 0;
	while(to_pars[i] == ' ')
		i++;
	while(to_pars[i])
	{
		if(to_pars[i] == '"')
		{
			if(flag == 0 || flag == 2)
				flag = 1;
			else if(flag == 1)
				flag = 2;
			i++;
		}
		else if(to_pars[i] == ' ' && flag == 1)
		{
			new_cmd[len] = to_pars[i];
			len++;
			i++;
		}
		else if(to_pars[i] == ' ' && flag == 0 || to_pars[i] == ' ' && flag == 2)
			break;
		else
		{
			new_cmd[len] = to_pars[i];
			len++;
			i++;
		}
	}
	new_cmd[len] = '\0';
	if(check_cmd_quotes(new_cmd) == 0)
	{
		free(new_cmd);
		return(0);
	}
	free(new_cmd);
	return(1);;
	// La on est caler sur le debut du mot qu'on va devoir copier

}

int to_pars_main(char *to_pars)
{
	char **pars_cmd;
	int ret_val;

	if(check_quotes(to_pars) == 1)
	{
		if(quotes_mod_bis(to_pars) == 0)
			return(0);
		printf("%d\n", quotes_mod_bis(to_pars));
	}
	if(pars_except(to_pars) == 0)
		return(0);
	pars_cmd = ft_split(to_pars, ' ');
	ret_val = first_element(pars_cmd[0]); 
    if(ret_val == 1)
	{
		free_array(pars_cmd);
		return(1);
	}
	else if(ret_val > 1)
    {
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
		if(ret_val == 9)
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
		if(ret_val == 10)
		{	
			if(pars_dolar(pars_cmd) == 0)
			{
				free_array(pars_cmd);
				return(2);
			}
			free_array(pars_cmd);
			return(1);
		}
		free_array(pars_cmd);
		return(1);
    }
	free_array(pars_cmd);
	return(0);
}


char *make_to_pars(t_shell *shell, int i)
{
	char *to_pars;
	int j;
	int k;
	int k_save;

	k = 0;
	j = 0;
	k_save = 0;
	while(j != i && shell->cmd[k])
	{
		if(shell->cmd[k] == ' ')
		{
			j++;
			while (shell->cmd[k] == ' ')
				k++;
		}
		else 
			k++;
	}
	//On arrive pixel la ou le pipe est.
	j = 0;
	if(shell->cmd[k] != '\0')
		k++;
	k_save = k;
	while (shell->cmd[k] && shell->cmd[k] != '|')
	{	
		j++;
		k++;
	}
	to_pars = malloc(sizeof(char) * j);
	j = 0;
	k = k_save;
	while (shell->cmd[k] && shell->cmd[k] != '|')
	{
		to_pars[j] = shell->cmd[k];
		j++;
		k++;
	}
	to_pars[j] = '\0';
	return(to_pars);
}

void token_parser(t_token *token, t_shell *shell, int len)
{
	char *to_pars;
	int return_val;
	int i;

	i = 0;
	return_val = 0;
	while(i < len)
	{
			// + Simple si c'est un pipe, decouper la commande et l'envoyer a verif de pipe 1 a prochain pipe, ou jusque rien
		if(token[i].id == 6)
		{
			to_pars = make_to_pars(shell, i);
			return_val = to_pars_main(to_pars);
			printf("%s\n", to_pars);
			if (return_val == 0)
				shell->error = 1;
			// i est l'equivalent au nombre d'espace trouver entre chaque partie de la commande.
			// par exemple, echo -n wdawdawd
			// 				0     1   	2
			// La regle pour creer la commande pourrait alors être
			// Prendre shell->cmd en entier, et tant que j != i avancer, il bougera l'index jusque au bon char
			// ensuite tant que shell->cmd a l'index trouver n'est pas egal a | ou '\0' avancer et copier
			i++;
		}
		i++;
	}
	
	// Parcourir toute l'array de token, checker si l'id du pipe n'est pas répèter
	return ; // -> in case of error
}
