LIBFT_NAME := libft.a
LIBFT_GH := https://github.com/josejpgg/libft_increment.git
LIBFT_PATH := ./lib/libft/
PROGRAM_NAME := minishell
CC = cc
FLAGS = 
# FLAGS = -Wall -Werror -Wextra
SOURCE := main.c env.c param.c \
safe_func.c interactive.c safe_free.c str_util.c
# READLINE := -lreadline -lncurses
READLINE := -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline
COMPILE := ${SOURCE:.c=.o}

all: minishell

# Regla para compilar archivos .c a .o (SIN -lreadline)
%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

# Regla para enlazar y generar el ejecutable (AQUÍ usamos -lreadline y libft.a)
minishell: $(COMPILE)
	$(CC) $(FLAGS) $(COMPILE) -L$(LIBFT_PATH) -lft $(READLINE) -o ${PROGRAM_NAME}

clean:
	@rm -rf $(COMPILE)
	@make clean -C $(LIBFT_PATH)

fclean:
	@rm -rf ${PROGRAM_NAME}
	@make fclean -C $(LIBFT_PATH)
	@rm -rf ${LIBFT_NAME}

re: fclean clean libft lib all

# Regla para compilar libft (se asegura de que existe y se compila correctamente)
lib:
	@if [ ! -f "$(LIBFT_PATH)$(LIBFT_NAME)" ]; then \
		make -C $(LIBFT_PATH); \
	fi

libft:
	@if [ ! -d "./lib" ]; then \
		mkdir -p lib; \
	fi
	@if [ -d "./lib/libft" ]; then \
		rm -rf ./lib/libft; \
	fi
	git clone $(LIBFT_GH) $(LIBFT_PATH)

test: all clean
	# echo 'grep hi "<infile"' | ./minishell
	# echo 'grep hi "<infile" <         ./test_files/infile' | ./minishell
	echo '' | ./minishell

leaksMac:
	export MallocStackLogging=1
	leaks --atExit --list -- ./minishell

leaksCampus:
	valgrind --leak-check=yes ./minishell

.PHONY: all clean fclean re lib

# Eliminé $(READLINE) en la compilación (%.o: %.c)
# Aseguré que libft.a se enlace correctamente con -L$(LIBFT_PATH) -lft