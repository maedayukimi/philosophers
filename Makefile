# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mawako <marvin@42.fr>                      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/07/27 15:00:00 by mawako            #+#    #+#              #
#    Updated: 2025/07/31 19:04:42 by mawako           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= philo

CC		= cc
CFLAGS	= -Wall -Wextra -Werror -pthread -g
RM		= rm -f

SRC_DIR = .
SRC_FILES = main.c init.c philosopher.c log.c time.c monitor.c \
	    parsing.c error_utils.c

SRC		= $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ		= $(SRC:.c=.o)

INC		= -I.

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
