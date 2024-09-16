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

# Règle pour générer les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour générer l'exécutable
$(NAME): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $(NAME)
	@echo "\033[33;32mCompilation finish with no problem ✅"

# Règle pour nettoyer les fichiers objets
clean:
	$(RM) $(OBJECTS)

# Règle pour nettoyer les fichiers objets et l'exécutable
fclean: clean
	$(RM) $(NAME)
	@echo "\033[33;36mAll the file has been deleted correctly ✅"

# Règle pour tout construire
all: $(NAME)

# Règle pour reconstruire le projet
re: fclean all

.PHONY: all clean fclean re

.SILENT:
