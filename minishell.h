#ifndef MINISHELL
# define MINISHELL
// -- Include of the external lib -- //
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/wait.h>
// -- End of include -- //
// -------------------- //
// # -- STRUCTURE -- # //
typedef struct t_shell
{
    char **cmd_token;
}    t_shell;

typedef struct t_token
{
    char *str;
    int id;
}    t_token;
// -- End -- //

// -- Declaration of external function -- //

// src/parsing/parsing_main.c //
int parsing_main(char *cmd);
int first_element(char *cmd);

// src/parsing/pars_first.c //
int check_bin(char *command);

// src/parsing/pars_cd.c //
int pars_cd(char **pars_cmd);

// src/parsing/pars_export.c //
int pars_export(char **pars_cmd);

// src/parsing/pars_env.c //
int pars_syntax(char **pars_cmd);

// src/parsing/pars_dolar.c //
int pars_dolar(char **pars_cmd);

// src/utils.c //
int ft_strlen(char *str);
int str_cmp(char *cmd, char *cmp_cmd);
void free_array(char **arr);
int	ft_isalnum(int alph);


// src/micro_lib/ft_split.c //
char	**ft_split(char *s, char c);



// -- End of declaration -- //
#endif