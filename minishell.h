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
# include <dirent.h>
// -- End of include -- //
// -------------------- //
// # -- STRUCTURE -- # //
typedef struct t_shell
{
	char *cmd;
	int	len_token;
}	t_shell;

typedef struct t_token
{
    char *str; 
    int id;
}    t_token;
// -- End -- //

// -- Declaration of external function -- //

// -- Parsing -- //
// src/parsing/parsing_main.c //
int parsing_main(t_shell *shell);
// src/parsing/pars_first.c //
int first_element(char *cmd);
// src/parsing/pars_check.c //
int check_quotes(t_shell *shell);
int check_bin(char *command);
int check_dolar(char *cmd);
int check_syntax(char *cmd);

// src/parsing/pars_cd.c //
int pars_cd(char **pars_cmd);
// src/parsing/pars_export.c //
int pars_export(char **pars_cmd);
// src/parsing/pars_env.c //
int pars_syntax(char **pars_cmd);
// src/parsing/pars_dolar.c //
int pars_dolar(char **pars_cmd);
// src/parsing/pars_char.c //
int quotes_mod(t_shell *shell);
int try_char(char *cmd);

// -- Token -- //
// src/token/token_main.c //
t_token *token_main(char *cmd, t_token *token, t_shell *shell);
// src/token/token_set.c //
void token_copy(t_token *token, char *cmd, int i);
void token_id(t_token *token, int i);
// src/token/token_id.c // 
int is_var(t_token *token, int i);
int is_char(t_token *token, int i);
int is_pipe(t_token *token, int i);
int is_infile(t_token *token, int i);
int is_outfile(t_token *token, int i);
int is_cmd(t_token *token, int i);
int is_flag(t_token *token, int i);
// src/token/token_utils.c //
void free_all(t_token *token, int len);
int len_for_token(char *cmd, int i);
int len_token(char *cmd);
void token_copy(t_token *token, char *cmd, int i);

// -- Executer -- //
// src/executer/exec_main.c //
void exec_main(t_token *token, char *cmd, char **envp, t_shell *shell);

// -- Builins -- // 
void echo(t_token *token, t_shell *shell);

// -- Micro lib -- //
// src/micro_lib/utils.c //
int ft_strlen(char *str);
int str_cmp(char *cmd, char *cmp_cmd);
int str_cmp_quotes(char *cmd, char *cmp_cmd);
void free_array(char **arr);
int	ft_isalnum(int alph);
char	*ft_strdup(char *src);
// src/micro_lib/ft_split.c //
char	**ft_split(char *s, char c);



// -- End of declaration -- //
#endif