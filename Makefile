NAME 		= 	minishell

OBJS_DIR 	= 	objs/

SRCS_DIR 	= 	srcs/

SRC			=	minishell.c 		\
				utils/utils.c		\
				err/error.c			\
				cmds/cd.c			\
				cmds/pwd.c			\
				cmds/env.c			\
				cmds/export.c		\
				entry/sequence.c	\
				entry/sanitize.c 	\
				entry/sanitize_utils.c \
				termcap/init_and_interr_term.c \
				termcap/set_mode.c 			\
				termcap/termcaps.c

OBJ 		= 	$(SRC:.c=.o)

OBJS 		= 	$(addprefix $(OBJS_DIR), $(OBJ))

all:	$(NAME)

$(NAME): $(OBJS)
	$(MAKE) -C $(SRCS_DIR)ft_printf
	gcc -ltermcap -Wall -Wextra -Werror $(SRCS_DIR)ft_printf/libftprintf.a -o $(NAME) $(OBJS)

$(OBJS_DIR)%.o :	$(SRCS_DIR)%.c
		@mkdir -p $(OBJS_DIR)
		@mkdir -p $(OBJS_DIR)entry
		@mkdir -p $(OBJS_DIR)path
		@mkdir -p $(OBJS_DIR)termcap
		@mkdir -p $(OBJS_DIR)cmds
		@mkdir -p $(OBJS_DIR)err
		@mkdir -p $(OBJS_DIR)utils
		@echo "Compiling: $<"
		@gcc -Wall -Wextra -Werror -I./incs/ -c $< -o $@

all: $(NAME)

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) clean -C $(SRCS_DIR)libft
	$(MAKE) clean -C $(SRCS_DIR)ft_printf
	$(MAKE) clean -C $(SRCS_DIR)termcap
	
fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(SRCS_DIR)libft
	$(MAKE) fclean -C $(SRCS_DIR)ft_printf
	$(MAKE) fclean -C $(SRCS_DIR)termcap
	
re: fclean all

run: all
	@./$(NAME)