NAME = pipex

#-------------DIRECTORIES------------#
INC_DIR = ./includes
LIB_DIR = ./libft
SRCS_DIR = ./srcs

#---------------FILES----------------#
SRCS = pipex_main.c pipex_bonus.c pipex_paths.c special_split.c

OBJS = $(addprefix objs/, ${SRCS:.c=.o})
DEPS = $(addprefix objs/, ${SRCS:.c=.d})

#-------------COMPILATION------------#
CC = gcc
CFLAGS = -Wall -Werror -Wextra

#---------------RULES----------------#


fclean: clean

re: fclean all

.PHONY: all clean fclean re bonus
