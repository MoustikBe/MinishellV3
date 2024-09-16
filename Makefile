NAME		= minishell
CC			= cc
CFLAGS		= -lreadline -c # -Wall -Werror -Wextra
RM			= rm -rf

SOURCES		=	src/parsing/pars_cd src/parsing/pars_char src/parsing/pars_dolar src/parsing/pars_export src/parsing/pars_first src/parsing/pars_syntax src/parsing/pars_main \
				src/token/token_main src/token/token_id src/token/token_set	\
				src/executer/exec_main \
				src/micro_lib/ft_split \
				src/utils \
				main

OBJECTS     = $(SOURCES:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) $< -o $@

$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(MLXFLAGS) -o $(NAME)
	@echo "\033[33;32mCompilation finish with no problem ✅"

clean:
	$(RM) $(OBJECTS)

fclean: clean
	$(RM) $(NAME)
	@echo "\033[33;36mAll the file his delete correctly ✅"

all : $(NAME)

re: fclean all

.PHONY: all clean fclean re

.SILENT:
