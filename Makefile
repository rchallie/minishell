NAME 		= 	minishell

OBJS_DIR 	= 	objs/

SRCS_DIR 	= 	srcs/

SRC			=	minishell.c 		\
				minishell_2.c 		\
				utils/utils.c		\
				err/error.c			\
				cmds/check.c		\
				cmds/cd.c			\
				cmds/pwd.c			\
				cmds/exit.c			\
				cmds/env.c			\
				cmds/echo.c			\
				otps/output.c		\
				specs/check.c		\
				exec/exec.c			\
				entry/sequence.c	\
				entry/sanitize.c 	\
				entry/sanitize_utils.c \
				entry/reorder.c	\
				termcap/termcap.c		\
				termcap/init_and_interr_term.c \
				termcap/set_mode.c 			\
				termcap/move_cursor.c		\
				termcap/move_cursor_2.c		\
				termcap/match_inputs.c		\
				termcap/utils_cursor.c		\
				termcap/ins_del_char.c		\
				termcap/reset_line.c		\
				termcap/ctrl_stuff.c		\
				termcap/tc_putchar.c

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
		@mkdir -p $(OBJS_DIR)otps
		@mkdir -p $(OBJS_DIR)specs
		@mkdir -p $(OBJS_DIR)exec
		@mkdir -p $(OBJS_DIR)launch
		@echo "Compiling: $<"
		@gcc -Wall -Wextra -Werror -I./incs/ -c $< -o $@

all: $(NAME)

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) clean -C $(SRCS_DIR)libft
	$(MAKE) clean -C $(SRCS_DIR)ft_printf
	
fclean: clean
	rm -f $(NAME)
	$(MAKE) fclean -C $(SRCS_DIR)libft
	$(MAKE) fclean -C $(SRCS_DIR)ft_printf
	
re: fclean all

run: all
	@./$(NAME)