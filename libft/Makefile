NAME = libft.a

SRCS = ft_strlen.c \
		ft_split.c \
		ft_strjoin.c \
		ft_strncmp.c \
		ft_strnstr.c \
		ft_substr.c \
		ft_strdup.c \
		ft_strlcpy.c \
		get_next_line.c \
		get_next_line_utils.c 

OBJS = ${SRCS:.c=.o}

DEPS = ${SRCS:.c=.d}

AR = ar rcs

RM = rm -rf

CC = gcc

CFLAGS = -Wall -Wextra -Werror

all: ${NAME}

%.o:%.c
		${CC} ${CFLAGS} -c $< -MMD

${NAME}: ${OBJS} Makefile
		${AR} ${NAME} ${OBJS}

clean:
		${RM} ${OBJS} ${DEPS}

fclean: clean
		${RM} ${NAME}

re: fclean all

-include ${DEPS}

.PHONY: all clean fclean re