#include "../../minishell.h"

/*
char *expanding_cmd(t_shell *shell)
{
	int i = 0;
	int i_copy = 0;
	int j = 0;
	int len = 0;
	char *cmp_cmd;

	// CONNAITRE la len de la nouvelle commande. donc
	while (shell->cmd[i])
	{
		if(shell->cmd[i] == '$')
		{
			i++;
			i_copy = i;
			while (shell->cmd[i] && shell->cmd[i] != ' ')
			{
				i++;
				j++;
			}
			cmp_cmd = malloc(sizeof(char) * j + 1);
			j = 0;
			while (shell->cmd[i_copy] && shell->cmd[i_copy] != ' ')
			{
				cmp_cmd[j] = shell->cmd[i_copy];
				j++;
				i_copy++;
			}
			printf("cmp_cmd -> %s\n", cmp_cmd);
		}
		i++;
	}
}
*/


int search_in_env(t_shell *shell, char *cmp_cmd)
{
	t_env *env_v;
	char *cmp_cmd_2;

	env_v = shell->env;
	while (env_v)
	{
		cmp_cmd_2 = build_cmp(env_v->env_var);
		//printf("cmp_1 -> %s cmp_2 -> %s\n", cmp_cmd, cmp_cmd_2);
		if(str_cmp(cmp_cmd, cmp_cmd_2) == 1)
		{
			//printf("MATCH\n");
			return(ft_strlen(env_v->env_var) - ft_strlen(cmp_cmd_2));
		}
		free(cmp_cmd_2);
		env_v = env_v->next;
	}
	return(0);
}

int copy_env_cmd(int i, t_shell *shell)
{
	t_env *env_v;
	char *cmp_cmd_1;
	char *cmp_cmd_2;
	int j = 0;
	int len = 0;

	env_v = shell->env;
	while (env_v)
	{
		//BUILDING OTHER
		j = i;
		while (shell->cmd[i] && shell->cmd[i] != ' ' && shell->cmd[i] != '"')
		{
			len++;
			i++;
		}
		i = j;
		len = 0;
		cmp_cmd_1 = malloc(sizeof(char) * len + 1);
		i++;
		while (shell->cmd[i] && shell->cmd[i] != ' ' && shell->cmd[i] != '"')
		{
			cmp_cmd_1[len] = shell->cmd[i];
			len++;
			i++;
		}
		cmp_cmd_1[len] = '\0';
		i = j;
		j = 0;
		len = 0;
		cmp_cmd_2 = build_cmp(env_v->env_var);
		//printf("cmd_1 %s\n", cmp_cmd_1);
		//printf("cmp_1_V -> %s cmp_2_V -> %s\n", cmp_cmd_1, cmp_cmd_2);
		if(str_cmp(cmp_cmd_1, cmp_cmd_2) == 1)
		{
		//	printf("MATCH_V\n");
			while(env_v->env_var[len] != '=')
				len++;
			len++;
			while (env_v->env_var[len])
			{
				//printf("COPIE EN COURS \n");
				shell->env_cmd[i] = env_v->env_var[len];
				i++;
				j++;
				len++;
			}
			return (j);
		}
		else
		{
			free(cmp_cmd_2);
			env_v = env_v->next;
		}
	}
	return(0);
}


void expansion(t_shell *shell)
{
	// IL FAUT PARCOURIR LA shell->cmd ET detecter si jamais il y'a une variable d'env.
	// SA detecte les dolars si on envois un char * -> check_cmd_quotes();
	// FAIRE UN SPLIT de la cmd. Si la fonction renvois 10. -> il faut changer la commande. 
	
	// TENTATIVE // 
	int i = 0;
	int i_copy = 0;
	int j = 0;
	int len = 0;
	int ret_val = 0;
	int flag = 0;
	char *cmp_cmd;

	// CONNAITRE la len de la nouvelle commande. donc
	while (shell->cmd[i])
	{
		// TROUVER UN MOYEN DE SAVOIR SI IL S'AGIT DE $?
		if(shell->cmd[i] == '$')
		{
			if(i > 0 && shell->cmd[i] == '$' && shell->cmd[i - 1] == '\'')
				return ;
			if(shell->cmd[i + 1] == '?' && shell->cmd[i + 2] == ' ')
				return ;
			flag = 1;
			i++;
			i_copy = i;
			while (shell->cmd[i] && shell->cmd[i] != ' ')
			{
				i++;
				j++;
			}
			cmp_cmd = malloc(sizeof(char) * j + 1);
			j = 0;
			while (shell->cmd[i_copy] && shell->cmd[i_copy] != ' ')
			{
				cmp_cmd[j] = shell->cmd[i_copy];
				j++;
				i_copy++;
			}
			cmp_cmd[j] = '\0';
			// PARFAITE CMP_CMD DECOUPER 
			// MNT essaye de trouver une correspondance et si il y'a une correspondance 
			// RENVOYER 1 SI PAS 0
			len = len + search_in_env(shell, cmp_cmd);
			//printf(" len_search -> %d\n", search_in_env(shell, cmp_cmd));
		}
		else
		{
		//	printf("char -> %c\n", shell->cmd[i]);
			len++;
			i++;
		}
	}
//	printf(" len -> %d\n", len);
	if(flag != 1)
		return ;
	shell->env_cmd = malloc(sizeof(char) * len);
	// CORRECTE LEN TROUVER// 
	// COPIE MNT //
	i = 0;
	i_copy = 0;
	j = 0;
	while (shell->cmd[i])
	{
		if(shell->cmd[i] == '$')
		{
			i_copy = i;
			while (shell->cmd[i] && shell->cmd[i] != ' ' && shell->cmd[i] != '"')
				i++;
			j = j + copy_env_cmd(i_copy, shell);

		}
		else
		{
			shell->env_cmd[j] = shell->cmd[i];
			j++;
			i++;
		}
	}
	shell->env_cmd[j] = '\0';
	free(shell->cmd);
	shell->cmd = ft_strdup(shell->env_cmd);
	//printf("shell cmd -> %s\n", shell->cmd);
	free(shell->env_cmd);
}

