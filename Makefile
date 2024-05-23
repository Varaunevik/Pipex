# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: vaunevik <vaunevik@student.42barcel>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/05/21 10:59:17 by vaunevik          #+#    #+#              #
#    Updated: 2024/05/23 13:13:39 by vaunevik         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
NAME		= pipex
INC			= ./includes/
INC_HEADERS	= $(INC)pipex.h

LIB_INC		= $(LIBFT)/libft.h

LIBFT			= ./libft/
FT_LNK		= -L$(LIBFT) -lft
FT_LIB		= $(LIBFT)libft.a

SRC_DIR		= srcs/
OBJ_DIR		= obj/
CFLAGS		= -I $(INC) -Wall -Werror -Wextra #-fsanitize=address
RM			= rm -f

###############COLORS###############

GREEN =			\033[0;92m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
DEF_COLOR = \033[0m


###############FILES################
SRC_FILES	=	pipex_main.c \
				pipex_paths.c \
				special_split.c \
				pipex_utils.c

SRC			=	$(addprefix $(SRC_DIR), $(SRC_FILES))
OBJ 		=	$(addprefix $(OBJ_DIR), $(SRC:.c=.o))
DEP			= 	$(addsuffix .d, $(basename $(OBJ)))
B_OBJ		=	$(OBJ)

##############RULES################
all:
		@$(MAKE) -C $(LIBFT)
		@$(MAKE) $(NAME)



$(OBJ_DIR)%.o: %.c Makefile $(FT_LIB) $(INC_HEADERS)
			@mkdir -p $(dir $@)
			@echo "$(MAGENTA)Compiling $(CYAN)$< $(DEF_COLOR)"
			@$(CC) $(CFLAGS) -c $< -o $@

$(NAME):	$(OBJ)
			@$(CC) $(CFLAGS) $(OBJ) $(FT_LNK) -o $(NAME)
			@echo "$(GREEN)\nCreated ${NAME} $(DEF_COLOR)\n"

-include $(DEP)

bonus: all

clean:
			@$(RM) -rf $(OBJ_DIR)
			@make clean -C $(LIBFT)
			@echo "\n $(MAGENTA)Objects cleaned successfully $(DEF_COLOR)\n"

fclean:		clean
			@$(RM) -f $(NAME)
			@make fclean -C $(LIBFT)
			@echo "\n $(MAGENTA)Objects and executable cleaned successfully $(DEF_COLOR)\n"

re:			fclean all
			@echo "$(GREEN)Cleaned and rebuilt everything!$(DEF_COLOR)"


.PHONY:		all clean fclean re
