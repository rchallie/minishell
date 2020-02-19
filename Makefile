NAME 		= 	minishell
OBJS_DIR 	= 	objs/
SRCS_DIR 	= 	srcs/
SRC			=	minishell.c \
				entry/sanitize.c
OBJ 		= 	$(SRC:.c=.o)
OBJS 		= 	$(addprefix $(OBJS_DIR), $(OBJ))

all:	$(NAME)

$(NAME): $(OBJS)
	$(MAKE) bonus -C $(SRCS_DIR)libft
	gcc -Wall -Wextra -Werror -o $(NAME) srcs/libft/libft.a $(OBJS)

$(OBJS_DIR)%.o :	$(SRCS_DIR)%.c
		mkdir -p $(OBJS_DIR)
		mkdir -p $(OBJS_DIR)entry
		echo "Compiling: $<"
		gcc -Wall -Wextra -Werror -I./incs/ -c $< -o $@

all: $(NAME)

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) clean -C $(SRCS_DIR)libft
	
fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(SRCS_DIR)libft
	
re: fclean all