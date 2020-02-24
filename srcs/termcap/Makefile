# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thervieu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/02/20 20:29:29 by thervieu          #+#    #+#              #
#    Updated: 2020/02/20 20:29:30 by thervieu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME = termcaps

CC = gcc

CFLAGS = -Wall -Wextra -Werror

INCLUDES = -I./termcaps.h

LIBFT_PATH = ./libft/

LIBFT_NAME = libft.a

SRC = ./init_and_interr_term.c \
		./set_mode.c    \
		./termcaps.c

OBJ = $(SRC:.c=.o)

all: $(NAME)

bonus: $(NAME)

$(NAME): libft_recipe $(OBJ)
	$(CC) -ltermcap $(CFLAGS) $(INCLUDES) $(MLX) $(OBJ) $(LIBFT_PATH)$(LIBFT_NAME) -o $(NAME)

libft_recipe:
	make -C $(LIBFT_PATH) all
	cp $(LIBFT_PATH)$(LIBFT_NAME) ./$(NAME)

clean:
	@echo "Remove .o  ..."
	@/bin/rm -f $(OBJ)

fclean: clean
	@/bin/rm -f $(NAME)
	@/bin/rm -f cub3D.bmp

re: fclean all

