# Nombre del programa final
NAME := minishell

# Compilador y flags
CC := cc
CFLAGS := -Wall -Wextra -Werror

# Directorio y nombre de la libft
LIBFT_DIR := ./lib/libft
LIBFT := $(LIBFT_DIR)/libft.a

# Librerías necesarias
READLINE := -I/usr/local/opt/readline/include -L/usr/local/opt/readline/lib -lreadline

# Archivos fuente del minishell
SRCS := main.c env.c param.c \
	safe_func.c interactive.c safe_free.c str_util.c \
	command.c line.c pipe.c redirection.c token.c \
	cmd_controller.c cmd_impl.c vector.c cmd_util.c \
	history.c history2.c

OBJS := $(SRCS:.c=.o)

# Regla por defecto
all: $(LIBFT) $(NAME)

# Regla para compilar minishell
$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)

# Compila libft usando su propio Makefile
$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

# Cómo compilar los .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Limpiar objetos del proyecto y de libft
clean:
	@rm -f $(OBJS)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re