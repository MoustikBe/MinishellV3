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
	new = malloc(sizeof(t_env));
	if(!new)
		return ;
	new->env_var = ft_strdup(str);
	new->next = NULL;
		env_v = shell->env;
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
	if(!token[j + 1].str)
	{
		t_env *env;

		env = shell->env;
		while (env != NULL)
		{
			printf("declare -x ");
			printf("%s\n", env->env_var);
			env = env->next;
		}
	}
	while(token[j].str && token[j].id != 6)
	{
		// ADDING IN THE ENV // 
		//printf("Adding -> %d\n", token[j].str);
		if(str_cmp(token[j].str, " ") == 1)
			j++;
		else
		{
			add_to_env(shell, token[j].str);
			j++;
		}
	}
	return ;
}

// Faire un parsing correcte sur le export !! 

// Petit bug sur l'export, il faut ajouter que SI
// Il y'a deja l'identifiant de la valeur changer seulement la valeur,
// Et garder l'identifiant mais en changer la valeur 
