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
	int in_quote;

	in_quotes = 0;
	in_quote = 0;
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
		else if(s[count] == '\'' && in_quote == 0)
			in_quote = 1;
		else if(s[count] == '\'' && in_quote == 1)
			in_quote = 0;
		// Si le char S est different de espace et que check = 0, on compte 1 mot
		else if (s[count] != c && check == 0)
		{
			check = 1;
			word++;
		}
		// Sinon si le char S est egal a espace check devient 0
		else if ((s[count] == c && in_quotes == 0 && in_quote == 0) || (s[count] == c && in_quote == 0 && in_quotes == 0))
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
		if(str[s_start] == '"' || str[s_start] == '\'')
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
		if(str[start] == '"' || str[start] == '\'')
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
	int		in_quote;
	char	**split;

	in_quotes = 0;
	in_quote = 0;
	split = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!s || !split)
		return (NULL);
	i = -1;
	j = 0;
	index = -1;
	while (++i <= ft_strlen(s))
	{
		if (in_quotes == 2)
		{
			while(s[i] != ' ' && s[i] != '\0')
				i++;
			in_quotes = 0;
		}
		else if (in_quote == 2)
		{
			while(s[i] != ' ' && s[i] != '\0')
				i++;
			in_quote = 0;
		}
		if(s[i] == '"' && in_quotes == 0 && s[i] && in_quote == 0)
		{
			if(i > 0)
			{
				if(s[i - 1] == ' ')
					index = i;
			}
			in_quotes = 1;
		}
		else if(s[i] == '\'' && in_quote == 0 && s[i] && in_quotes == 0)
		{
			if(i > 0)
			{
				if(s[i - 1] == ' ')
					index = i;
			}
			in_quote = 1;
		}	
		else if(s[i] == '"' && in_quotes == 1)
			in_quotes = 2;
		else if(s[i] == '\'' && in_quote == 1)
			in_quote = 2;
		else if ((s[i] != c && s[i] != '"' && index < 0) || (s[i] != c && s[i] != '\'' && index < 0))
			index = i;
		else if (((s[i] == c && in_quotes == 0 || i == ft_strlen(s) || s[i] == '"' && in_quotes == 1 ) && index >= 0 && in_quote == 0) || ((s[i] == c && in_quote == 0 || i == ft_strlen(s) || s[i] == '\'' && in_quote == 1 ) && index >= 0 && in_quotes == 0))
		{
			split[j++] = word_dup(s, index, i);
			if (!split[j - 1])
				return (free_memory(split, j), NULL);
			index = -1;
		}	
	}
	split[j] = NULL;
	/*
	int i_c = 0;
	int j_c;
	while (split[i_c])
	{
		j_c = 0;
		while (split[i_c][j_c])
		{
			printf("char -> %c | ", split[i_c][j_c]);
			j_c++;
		}
		
		printf("\n");
		i_c++;
	}
	*/
	return (split);
}