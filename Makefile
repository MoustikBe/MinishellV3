NAME        = minishell
CC          = cc
CFLAGS      = # -Wall -Werror -Wextra 
LDFLAGS     = -lreadline 
RM          = rm -rf

SOURCES     = src/parsing/pars_cd.c src/parsing/pars_char.c src/parsing/pars_dolar.c src/parsing/pars_export.c src/parsing/pars_first.c src/parsing/pars_syntax.c src/parsing/pars_main.c \
              src/token/token_main.c src/token/token_id.c src/token/token_set.c \
              src/executer/exec_main.c \
              src/micro_lib/ft_split.c \
              src/utils.c \
              main.c

OBJECTS     = $(SOURCES:%.c=%.o)

%.o: %.c
	@printf "\033[0;33mGenerating minishell objects... %-33.33s\r" $@
	@$ $(CC) $(CFLAGS) -c $< -o $@

$(NAME): $(OBJECTS)
	@echo "\n"
	@echo "\033[0;32mCompiling minishell..."
	@ $(CC) $(OBJECTS) $(LDFLAGS) -o $(NAME) 
	@echo "\n\033[0mDone !"

clean:
	$(RM) $(OBJECTS)
	@echo "\033[33;36mClean ✅"

fclean: clean
	@echo "\033[33;36mAll the file has been deleted correctly ✅"
	@ $(RM) $(NAME)

all: $(NAME)

re: fclean all

.PHONY: all clean fclean re

