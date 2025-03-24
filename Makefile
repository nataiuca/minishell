LIBFT_NAME := libft.a
LIBFT_GH := https://github.com/josejpgg/libft_increment.git
LIBFT_PATH := ./libft/
PROGRAM_NAME := minishell
CC = cc
FLAGS =
# FLAGS = -Wall -Werror -Wextra
SOURCE := main.c env.c param.c \
safe_func.c interactive.c safe_free.c str_util.c \
command.c line.c pipe.c redirection.c token.c history.c history2.c \
expand.c
READLINE := -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline
COMPILE := ${SOURCE:.c=.o}

all: $(LIBFT_PATH)$(LIBFT_NAME) $(PROGRAM_NAME)

# Regla para compilar archivos .c a .o
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

# Regla para enlazar y generar el ejecutable (AQUÍ usamos -lreadline y libft.a)
$(PROGRAM_NAME): $(COMPILE) $(LIBFT_PATH)$(LIBFT_NAME)
	$(CC) $(FLAGS) $(COMPILE) -L$(LIBFT_PATH) -lft $(READLINE) -o $@

$(LIBFT_PATH)$(LIBFT_NAME):
	@if [ ! -d "$(LIBFT_PATH)" ]; then \
        mkdir -p $(LIBFT_PATH); \
        git clone $(LIBFT_GH) $(LIBFT_PATH); \
    fi
	@make -C $(LIBFT_PATH)

clean:
	@rm -rf $(COMPILE) $(PROGRAM_NAME)
	@if [ -d "$(LIBFT_PATH)" ]; then \
        @make clean -C $(LIBFT_PATH); \
    fi

fclean: clean
	@rm -rf $(LIBFT_PATH)$(LIBFT_NAME)

re: fclean all

test: all clean
    # echo 'grep hi "<infile"' | ./minishell
    # echo 'grep hi "<infile" <         ./test_files/infile' | ./minishell
	echo '' | ./minishell

leaksCampus:
    valgrind --leak-check=yes ./minishell

.PHONY: all clean fclean re libft
