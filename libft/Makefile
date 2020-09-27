# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: thervieu <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/11/19 13:06:37 by thervieu          #+#    #+#              #
#    Updated: 2019/11/19 13:06:39 by thervieu         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

CC = gcc

CFLAGS = -Wall -Wextra -Werror -g

SRC =	ft_atoi.c		\
		ft_bzero.c		\
		ft_calloc.c		\
		ft_isalnum.c	\
		ft_isalpha.c	\
		ft_isascii.c	\
		ft_isdigit.c	\
		ft_isprint.c	\
		ft_itoa.c		\
		ft_memalloc.c	\
		ft_memccpy.c	\
		ft_memchr.c		\
		ft_memcmp.c		\
		ft_memcpy.c		\
		ft_memdel.c		\
		ft_memmove.c	\
		ft_memset.c		\
		ft_putchar_fd.c	\
		ft_putchar.c	\
		ft_putendl_fd.c	\
		ft_putendl.c	\
		ft_putnbr_fd.c	\
		ft_putnbr.c		\
		ft_putstr_fd.c	\
		ft_putstr.c		\
		ft_strcat.c		\
		ft_strchr.c		\
		ft_strclr.c		\
		ft_strcmp.c		\
		ft_strcpy.c		\
		ft_strdel.c		\
		ft_strdup.c		\
		ft_strequ.c		\
		ft_striter.c	\
		ft_striteri.c	\
		ft_strjoin.c	\
		ft_strlcat.c	\
		ft_strlcpy.c	\
		ft_strlen.c		\
		ft_strmap.c		\
		ft_strmapi.c	\
		ft_strncat.c	\
		ft_strncmp.c	\
		ft_strncpy.c	\
		ft_strnequ.c	\
		ft_strnew.c		\
		ft_strnstr.c	\
		ft_strnlen.c	\
		ft_strrchr.c	\
		ft_strstr.c		\
		ft_split.c		\
		ft_substr.c		\
		ft_strtrim.c	\
		ft_tolower.c	\
		ft_toupper.c	\
		get_next_line.c

SRC_BONUS =	ft_lstadd_back.c	\
		ft_lstadd_front.c	\
		ft_lstclear.c		\
		ft_lstdelone.c		\
		ft_lstiter.c		\
		ft_lstlast.c		\
		ft_lstmap.c			\
		ft_lstnew.c			\
		ft_lstsize.c		

OBJ = $(SRC:.c=.o)

OBJ_BONUS = $(SRC_BONUS:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

all : $(NAME)

$(NAME): $(OBJ)
	@ar rcs $@ $^

bonus: $(OBJ) $(OBJ_BONUS)
	@ar rcs $(NAME) $^

clean:
	@echo "Remove .o  ..."
	@/bin/rm -f $(OBJ) $(OBJ_BONUS)

fclean: clean
	@echo "Remove lib ..."
	@/bin/rm -f $(NAME)

re: fclean all
