#include "../../minishell.h"

int direct_bin(char *cmd)
{
    int len;
    int flag_acces;

	//printf("|%s|\n", cmd);
    len = ft_strlen(cmd);
    if(len > 4)
    {
        if((cmd[0] == '/' && cmd[1] == 'b' && cmd[2] == 'i' && cmd[3] == 'n' && cmd[4] == '/' )|| (cmd[0] == '/' && cmd[1] == 'u' && cmd[2] == 's' && cmd[3] == 'r' && cmd[4] == '/'))
        {
            if(access(cmd, O_RDONLY) == 0)
                return(1);
        }
        return(0);
    }
    return(0);
}
