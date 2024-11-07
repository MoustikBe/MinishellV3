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
	/* IN TEST */
	int fd[2];
	int fd_temp;
	int nb_cmd;
	int index;
	/* IN TEST */
	int	len_token;
	int error;
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
int parsing_main(char *cmd);
int pars_except(char *cmd);
// src/parsing/pars_bin.c //
int direct_bin(char *cmd);
// src/parsing/pars_check.c //
int check_quotes(char *cmd);
int check_bin(char *command);
int check_dolar(char *cmd);
int check_syntax(char *cmd);
// src/parsing/pars_cd.c //
int pars_cd(char **pars_cmd);
// src/parsing/pars_cmd.c // 
int check_cmd_quotes(char *cmd_check);
// src/parsing/pars_export.c //
int pars_export(char **pars_cmd);
// src/parsing/pars_env.c //
int pars_syntax(char **pars_cmd);
// src/parsing/pars_dolar.c //
int pars_dolar(char **pars_cmd);
// src/parsing/pars_char.c //
int check_bin_quotes(char *command);

// -- Token -- //
// src/token/token_main.c //
t_token *token_main(char *cmd, t_token *token, t_shell *shell);
// src/token/token_set.c //
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
void token_copy(t_token *token, char *cmd, int i, int j);
// src/token/token_pars.c // 
void token_parser(t_token *token, t_shell *shell, int len);

// -- Executer -- //
// src/executer/exec_main.c //
void exec_main(t_token *token, char *cmd, char **envp, t_shell *shell);
char *make_path(char *token);
// src/executer/exec_pipe.c //
void pipex_simple(t_token *token, t_shell *shell);
// src/executer/exec_pipe_multi.c //
void pipex_multi(t_token *token, t_shell *shell);

// -- Builins -- // 
void echo(t_token *token, int j);
void pwd(void);
void cd(char *path);

// -- Micro lib -- //
// src/micro_lib/utils.c //
int ft_strlen(char *str);
int str_cmp(char *cmd, char *cmp_cmd);
int str_cmp_quotes(char *cmd, char *cmp_cmd);
void free_array(char **arr);
int	ft_isalnum(int alph);
char	*ft_strdup(char *src);
char *clean_name(char *src);
// src/micro_lib/ft_split.c //
char	**ft_split(char *s, char c);
char	*ft_strjoin(char *s1, char *s2);
// src/micro_lib/split_basic.c //
char	**ft_split_basic(char *s, char c);

// -- End of declaration -- //
#endif