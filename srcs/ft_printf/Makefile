# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/27 13:29:35 by thervieu          #+#    #+#              #
#    Updated: 2020/05/29 18:06:24 by excalibur        ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libftprintf.a

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

LIBFT_PATH = ../libft/

LIBFT_NAME = libft.a

SRC = 	ft_printf.c				\
		is_x.c					\
		manage_char_and_str.c	\
		manage_flags.c			\
		manage_int_and_ptr.c	\
		manage_percent.c		\
		manage_uint_and_hexa.c	\
		parse_flags.c			\
		put_group_functions.c

OBJ = $(SRC:.c=.o)

%.o: $.c 
	$(CC) $(CFLAGS) $(LIBFT_PATH) -o $@ -c $<

all: $(NAME)

$(NAME): libft_recipe $(OBJ)
	ar rcs $(NAME) $(OBJ) 

libft_recipe:
	make -C $(LIBFT_PATH) bonus
	cp $(LIBFT_PATH)$(LIBFT_NAME) ./$(NAME)
clean:
	@echo "Remove .o  ..."
	@/bin/rm -f $(OBJ)

fclean: clean
	@echo "Remove libftprintf.a ..."
	@/bin/rm -f $(NAME)

re: fclean all
