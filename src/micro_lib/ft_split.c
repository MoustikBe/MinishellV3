/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: misaac-c <misaac-c@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 20:58:23 by misaac-c          #+#    #+#             */
/*   Updated: 2024/11/26 11:07:59 by misaac-c         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../minishell.h"

static void free_memory(char **split, int j)
{
    while (--j >= 0)
        free(split[j]);
    free(split);
}

static int count_words(char *s, char c)
{
    int count = 0;
    int in_dquote = 0;
    int in_squote = 0;
    
    for (int i = 0; s[i]; i++)
    {
        if (s[i] == '"' && !in_squote)
            in_dquote = !in_dquote;
        else if (s[i] == '\'' && !in_dquote)
            in_squote = !in_squote;
        else if (s[i] != c && !in_dquote && !in_squote && (i == 0 || s[i - 1] == c))
            count++;
    }
    return count;
}

static char *word_dup(char *str, int start, int finish)
{
    char *word = malloc((finish - start + 1) * sizeof(char));
    if (!word)
        return NULL;

    int i = 0;
    int in_dquote = 0;
    int in_squote = 0;

    for (int j = start; j < finish; j++)
    {
        if (str[j] == '"' && !in_squote)
            in_dquote = !in_dquote;
        else if (str[j] == '\'' && !in_dquote)
            in_squote = !in_squote;
        else
            word[i++] = str[j];
    }
    word[i] = '\0';
    return word;
}

char **ft_split(char *s, char c)
{
    int i = 0;
    int j = 0;
    int index = -1;
    int in_dquote = 0;
    int in_squote = 0;
    char **split = malloc((count_words(s, c) + 1) * sizeof(char *));
    
    if (!s || !split)
        return NULL;

    while (s[i])
    {
        if (s[i] == '"' && !in_squote)
            in_dquote = !in_dquote;
        else if (s[i] == '\'' && !in_dquote)
            in_squote = !in_squote;

        if (s[i] != c && index == -1)
            index = i;
        else if ((s[i] == c && !in_dquote && !in_squote) || (s[i + 1] == '\0'))
        {
            if (index != -1)
            {
                split[j++] = word_dup(s, index, (s[i + 1] == '\0' && s[i] != c) ? i + 1 : i);
                if (!split[j - 1])
                    return (free_memory(split, j), NULL);
                index = -1;
            }
        }
        i++;
	}
	split[j] = NULL;
	return (split);
}
