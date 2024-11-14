#include "../../minishell.h"


int check_cmd(char *str)
{
	char *env_str;
	int i;
	int i_copy;
	int j;

	j = 0;
	i = 0;
	while (str[i] && str[i] != '=')
		i++;
	 if (str[i] != '=' || i == 0 || str[i + 1] == '\0')
		return(0);
	return(1);
}


void add_to_env(t_shell *shell, char *str)
{
	t_env *env_v;
	t_env *new;

	if(check_cmd(str) == 0)
		return ;
	env_v = shell->env;
	new = malloc(sizeof(t_env));
	if(!new)
		return ;
	new->env_var = ft_strdup(str);
	new->next = NULL;
	while (env_v->next)
		env_v = env_v->next;
	env_v->next = new;
}
/*
	t_env *env_v;
	t_env *new;
	int i;

	env_v = shell->env;
	i = 0;
	// TANT QUE envp a du contenu // 
	while (envp[i])
	{
		// CREER UN NODE ET Y AJOUTER LA VALEUR ACTUEL // 
		new = malloc(sizeof(t_env));
		new->env_var = ft_strdup(envp[i]);
		new->next = NULL;
		if (shell->env == NULL) 
        {
            shell->env = new;
            env_v = shell->env;
        }
        else
        {
            env_v->next = new;
            env_v = env_v->next;
        }
		i++;
	}

*/

void export(t_shell *shell, t_token *token, int j)
{

	j++;
	while(token[j].str && token[j].id != 6)
	{
		// ADDING IN THE ENV // 
		//printf("Adding -> %s\n", token[j].str);
		add_to_env(shell, token[j].str);
		j++;
	}
	return ;
}

// Faire un parsing correcte sur le export !! 