#include "../../minishell.h"

int direct_bin(char *cmd)
{
    int len;
    int flag_acces;

	//printf("|%s|\n", cmd);
    len = ft_strlen(cmd);
    if(len > 4)
    {
		if(access(cmd, O_RDONLY) == 0)
			return(1);
        return(0);
    }
    return(0);
}
