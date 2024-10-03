#include "../../minishell.h"

int direct_bin(char *cmd)
{
    int len;
    int flag_acces;

    len = ft_strlen(cmd);
    if(len > 4)
    {
        if(cmd[0] == '/' && cmd[1] == 'b' && cmd[2] == 'i' && cmd[3] == 'n' && cmd[4] == '/')
        {
            if(access(cmd, O_RDONLY) == 0)
                return(1);
        }
        return(0);
    }
    return(0);
}
