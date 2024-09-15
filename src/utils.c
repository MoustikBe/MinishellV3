#include "../minishell.h"

int ft_strlen(char *str)
{
	int i;

    i = 0;
    while (str[i])
		i++;
    return (i);
}

int str_cmp(char *cmd, char *cmp_cmd)
{
	int i;

	i = 0;
	while(cmd[i] && cmd[i] != ' ')
	{
		if(cmd[i] != cmp_cmd[i])
			return(0);
		i++;
	}
	if(ft_strlen(cmp_cmd) == i)
		return(1);
	return(0);
}

void free_array(char **arr)
{
	int i;

	i = 0;
	while(arr[i])
	{
          free(arr[i]);
          i++;
	}
	free(arr);
}

int	ft_isalnum(int alph)
{
	if (((alph >= 65) && (alph <= 90)) || ((alph >= 97) && (alph <= 122)))
		return (1);
	else if ((alph >= 48) && (alph <= 57))
		return (1);
	else
		return (0);
}

char	*ft_strdup(char *src)
{
	char	*dest;
	int		length;
	int		count;

	length = ft_strlen(src);
	count = 0;
	dest = malloc((length + 1) * sizeof(char));
	if (dest == NULL)
	{
		return (NULL);
	}
	while (src[count] != '\0')
	{
		dest[count] = src[count];
		count++;
	}
	dest[count] = '\0';
	return (dest);
}
