/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 20:58:23 by misaac-c          #+#    #+#             */
/*   Updated: 2024/09/26 12:50:55 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void	free_memory(char **split, int j)
{
	while (--j >= 0)
		free(split[j]);
	free(split);
}

static int	count_words(char *s, char c)
{
	int	count;
	int	word;
	int	check;
	int in_quotes;

	in_quotes = 0;
	count = 0;
	check = 0;
	word = 0;
	// pwd | "Hello wolrd"
	// Tant que le char S existe 
	while (s[count] != '\0')
	{
		if(s[count] == '"' && in_quotes == 0)
			in_quotes = 1;
		else if(s[count] == '"' && in_quotes == 1)
			in_quotes = 0;
		// Si le char S est different de espace et que check = 0, on compte 1 mot
		else if (s[count] != c && check == 0)
		{
			check = 1;
			word++;
		}
		// Sinon si le char S est egal a espace check devient 0
		else if (s[count] == c && in_quotes == 0)
			check = 0;
		// Sinon on conitnue a parcourir dans tous les cas 
		count++;
	}
	return (word);
}

static char	*word_dup(char *str, int start, int finish)
{
	char	*word;
	int 	s_start;
	int		new_alloc;
	int		i;

	s_start = start;
	new_alloc = 0;
	i = 0;
	while(s_start < finish)
	{
		if(str[s_start] == '"')
			s_start++;
		else
		{
			s_start++;
			new_alloc++;
		}
	}
	word = malloc((new_alloc + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while(start < finish)
	{
		if(str[start] == '"')
			start++;
		else
			word[i++] = str[start++];
	}	
	word[i] = '\0';
	return (word);
}

char	**ft_split(char *s, char c)
{
	size_t	i;
	size_t	j;
	int		index;
	int		in_quotes;
	char	**split;

	in_quotes = 0;
	// A CHANGER compter un mot quand on est sur " hello world " et pas 3
	// PARFAIT, -> changer // 
	split = malloc((count_words(s, c) + 1) * sizeof(char *));
	// p'tite vérif
	if (!s || !split)
		return (NULL);
	i = -1;
	j = 0;
	// je capte pas pq -1 mais alz oslm
	index = -1;
	// On parcour chaque char
	while (++i <= ft_strlen(s))
	{
		// Si le char s est different de espace, et index = -1, 
		// C'est a dire dans la plupart des cas, alors index devient 0
		
		if (in_quotes == 2)
		{
			while(s[i] != ' ' && s[i] != '\0')
				i++;
			in_quotes = 0;
		}
		if(s[i] == '"' && in_quotes == 0 && s[i])
		{
			if(i > 0)
			{
				if(s[i - 1] == ' ')
					index = i;
			}
			in_quotes = 1;
		}	
		else if(s[i] == '"' && in_quotes == 1)
			in_quotes = 2;
		else if (s[i] != c && s[i] != '"' && index < 0)
			index = i;
		// Sinon si le char est egale a espace ou que i est le dernier char et
		// que index est plus grand ou egal a 0, donc evite le cas ou le char s est egal a espace 
		// Il faut alors split dans le cas aussi ou c'est un quotes 
		else if ((s[i] == c && in_quotes == 0 || i == ft_strlen(s) || s[i] == '"' && in_quotes == 1 ) && index >= 0)
		{
			split[j++] = word_dup(s, index, i);
			if (!split[j - 1])
				return (free_memory(split, j), NULL);
			index = -1;
		}	
		printf("-----------------\nchar -> %c\n in -> %d\n i -> %zu\n index -> %d\n -----------------\n", s[i], in_quotes, i, index);
	}
	split[j] = NULL;
	return (split);
}

