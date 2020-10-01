# SIMPLE ===================================================================================

NAME 		= 	minishell

NAME_BONUS	=	minishell_bonus
# ==========================================================================================
# SRCS =====================================================================================

SRCS_DIR 	= 	srcs/
SRC			=	minishell.c 					\
				minishell_2.c 					\
				minishell_pipe.c 				\
				minishell_init.c 				\
				splitter.c 						\
				check.c							\
				exec.c							\
				error.c							\
				sig_catcher.c					\
				utils/utils.c					\
				utils/words_parser.c			\
				utils/utils_double_tabs.c		\
				utils/utils_double_tabs_2.c		\
				cmds/check.c					\
				cmds/cd.c						\
				cmds/pwd.c						\
				cmds/exit.c						\
				cmds/unset.c					\
				cmds/env.c						\
				cmds/echo.c						\
				cmds/export.c					\
				cmds/export_utils.c				\
				entry/sequence.c				\
				entry/sequence_tuning.c			\
				entry/sanitize.c 				\
				entry/sanitize_env.c 			\
				entry/reorder.c

SRC_BONUS	=	minishell_bonus.c 				\
				minishell_2.c 					\
				minishell_pipe.c 				\
				splitter.c 						\
				check.c							\
				exec_bonus.c					\
				error.c							\
				sig_catcher_bonus.c				\
				interface.c						\
				utils/utils_bonus.c				\
				utils/word_parser_bonus.c		\
				utils/utils_double_tabs.c		\
				utils/utils_double_tabs_2.c		\
				cmds/check.c					\
				cmds/cd.c						\
				cmds/pwd.c						\
				cmds/exit_bonus.c				\
				cmds/unset.c					\
				cmds/env.c						\
				cmds/echo.c						\
				cmds/export.c					\
				cmds/export_utils.c				\
				entry/sequence.c				\
				entry/sequence_tuning.c			\
				entry/sanitize_bonus.c 			\
				entry/sanitize_env.c 			\
				entry/reorder.c					\
				termcap/termcap.c				\
				termcap/init_and_interr_term.c 	\
				termcap/set_mode.c 				\
				termcap/move_cursor.c			\
				termcap/move_cursor_2.c			\
				termcap/match_inputs.c			\
				termcap/utils_cursor.c			\
				termcap/ins_del_char.c			\
				termcap/reset_line.c			\
				termcap/ctrl_stuff.c			\
				termcap/tc_putchar.c

# ==========================================================================================

# INCLUDES =================================================================================

INCS_DIR	 = ./incs

# ==========================================================================================

# FLAGS ====================================================================================

INCLUDE 	= -I$(INCS_DIR)
FLAGS 		= -Wall -Wextra -Werror -g
FLAGS_BONUS	= -ltermcap -Wall -Wextra -Werror -g

# ==========================================================================================

# OBJS =====================================================================================

OBJS_DIR 	= 	objs/
OBJ 		= 	$(SRC:.c=.o)
OBJS 		= 	$(addprefix $(OBJS_DIR), $(OBJ))
OBJ_BONUS 	= 	$(SRC_BONUS:.c=.o)
OBJS_BONUS 	= 	$(addprefix $(OBJS_DIR), $(OBJ_BONUS))

# ==========================================================================================

# COLORS ===================================================================================

_GREEN=$'\x1b[32m
_WHITE=$'\x1b[37m
_RED=$'\x1b[31m
_BOLD=$'\x1b[1m
_NORMAL=$'\x1b[0m
_UNDER=$'\x1b[4m
_YELLOW=$'\x1b[33m

# ==========================================================================================

# FROM SUBJECT =============================================================================

all:	$(NAME)

$(NAME): $(OBJS)
	make bonus -C ./libft
	clang $(FLAGS) $(INCLUDES) ./libft/*.o ./libft/ft_printf/*.o $(OBJS) -o $(NAME) 

$(NAME_BONUS): $(OBJS_BONUS)
	make bonus -C ./libft
	clang $(FLAGS_BONUS) $(INCLUDES) ./libft/*.o ./libft/ft_printf/*.o $(OBJS_BONUS) -o $(NAME_BONUS) 

$(OBJS_DIR)%.o :	$(SRCS_DIR)%.c $(INCS_DIR)/minishell.h
		@mkdir -p $(OBJS_DIR)
		@mkdir -p $(OBJS_DIR)entry
		@mkdir -p $(OBJS_DIR)path
		@mkdir -p $(OBJS_DIR)termcap
		@mkdir -p $(OBJS_DIR)cmds
		@mkdir -p $(OBJS_DIR)err
		@mkdir -p $(OBJS_DIR)utils
		@mkdir -p $(OBJS_DIR)otps
		@mkdir -p $(OBJS_DIR)exec
		@mkdir -p $(OBJS_DIR)launch
		@echo "Compiling: $<"
		@clang -Wall -Wextra -Werror -I./incs/ -c $< -o $@

clean:
	rm -rf $(OBJS_DIR)
	$(MAKE) clean -C ./libft
	
fclean: clean
	rm -f $(NAME)
	rm -f $(NAME_BONUS)
	$(MAKE) fclean -C ./libft
	
re: fclean all

bonus: $(NAME_BONUS)

# ==========================================================================================
