NAME = libft.a
SRC = ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c \
ft_isascii.c ft_isdigit.c ft_isprint.c ft_memchr.c ft_memcmp.c \
ft_memcpy.c ft_memmove.c ft_memset.c ft_strchr.c ft_strdup.c ft_strlcat.c \
ft_strlcpy.c ft_strlen.c ft_strncmp.c ft_strnstr.c ft_strrchr.c ft_tolower.c \
ft_toupper.c \
ft_itoa.c ft_putchar_fd.c ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c \
ft_split.c ft_striteri.c ft_strjoin.c ft_strmapi.c ft_strtrim.c \
ft_substr.c \
ft_printf.c ft_flag_bonus.c ft_print_fd.c ft_word.c ft_numeric.c ft_hexadecimal.c \
ft_isupper.c ft_islower.c get_next_line.c get_next_line_utils.c \
ft_exit_message.c ft_isnegnumber.c ft_isposnumber.c \
ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c \
ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c \
ft_strcmp.c ft_strstr.c ft_isspace.c
SRC_B = ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
ft_lstadd_back_bonus.c ft_lstdelone_bonus.c ft_lstclear_bonus.c \
ft_lstiter_bonus.c ft_lstlast_bonus.c ft_lstmap_bonus.c
FLAG = -Wall -Werror -Wextra
OBJ = $(SRC:.c=.o)
OBJ_B = $(SRC_B:.c=.o)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

all: $(NAME)

%.o: %.c
	cc $(FLAG) -c $< -o $@

clean:
	rm -f $(OBJ) $(OBJ_B)

fclean: clean
	rm -f $(NAME)

re: fclean all

bonus: $(OBJ_B) $(OBJ)
	ar rcs $(NAME) $(OBJ_B) $(OBJ)

.PHONY: clean, fclean, re, all
