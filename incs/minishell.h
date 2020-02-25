/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:02:29 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/25 16:31:06 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SUCCESS 1
# define ERROR 0

# include "../srcs/libft/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <stdio.h> //PLOP
# include <unistd.h>
# include <term.h>
# include <curses.h>
# include <termios.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include "../srcs/libft/libft.h"

# define HISTORY_PATH "save_history"
# define MAX_CMD_LEN 4096
# define MAX_KEY_LEN 7
# define SUCCESS 1
# define ERROR 0

# define KEY_CODE_UP "\x1b\x5b\x41\0"
# define KEY_CODE_DO "\x1b\x5b\x42\0"
# define KEY_CODE_RI "\x1b\x5b\x43\0"
# define KEY_CODE_LE "\x1b\x5b\x44\0"
# define KEY_CODE_CTRL_D "\x04"
# define KEY_CODE_CTRL_L "\x0c"
# define KEY_CODE_HOME "\x1b\x5b\x48"
# define KEY_CODE_END "\x1b\x5b\x46"
# define KEY_CODE_CTRL_LE "\x1b\x5b\x31\x3b\x35\x44"
# define KEY_CODE_CTRL_RI "\x1b\x5b\x31\x3b\x35\x43"
# define KEY_CODE_CTRL_UP "\x1b\x5b\x31\x3b\x35\x41"
# define KEY_CODE_CTRL_DO "\x1b\x5b\x31\x3b\x35\x42"

# define KEY_CTRL_LE -10
# define KEY_CTRL_RI -11
# define KEY_CTRL_UP -12
# define KEY_CTRL_DO -13
# define KEY_CTRL_D -20
# define KEY_CTRL_L -21

typedef struct		s_keymatch
{
	char			*key_code;
	int				key_ret;
}					t_key_match;

typedef struct		s_dlist
{
	void			*content;
	size_t			content_size;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}					t_dlist;

typedef struct		s_winsz
{
	size_t			row;
	size_t			col;
}					t_winsz;

typedef struct		s_line
{
	t_dlist			*hist;
	size_t			cursor;
	size_t			length;
	size_t			hist_depth;
	size_t			hist_size;
	t_winsz			winsz;
	t_winsz			start;
	char			cmd[MAX_CMD_LEN];
}					t_line;

typedef struct		s_keymove
{
	int				key;
	void			(*funct)(t_line *line);
}					t_keymove;

typedef struct		s_args
{
	struct s_args	*next;
	void			*content;
}					t_args;

typedef struct		s_minishell
{
	char			*entry;
	char			**treated;
	char			**envp;
	int				*sequence;
	int				seq_cursor;
	int				iscmdret;
	char			*output;
}					t_minishell;

void				init_terminal_data(void);
void				interrogate_terminal(void);
void				default_term_mode(void);
void				raw_term_mode(void);
void				free_double_char_tab(char **tab_to_free);

char				*add_char_to_word(char *word, char c);
char				*get_env_var_by_name(char *name, char **envp);

int					ft_printf(const char *str, ...);
int					ft_secure_strlen(const char *str);
int					ft_is_whitespace(char c);
int					get_pwd(char **pwd);
int					line_edition(char **entry);
int					get_pwd_short(char **pwd);
int					get_word(char *entry, char **word);
int					get_sequence(char **treated, int **sequence);
int					cd(t_minishell *ms);
int					print_work_dir(t_minishell *ms);
int					env(char **envp);
int					export(t_args *args);
int					error_path(const char *cmd, const char *path,
						int errnum);
int					error_identifier(char *cmd, const char *identifier,
						char *error);
int					error_command(char *cmd);
int					is_cmd(char *cmd);
int					sanitize(char *entry, char ***treated);
int					get_double_char_tab_len(char **tabl);
int					is_char_spec(char *s);

int		treat_output(t_minishell *ms);

/* ______ termcaps ______ */

int				line_edition(char **entry);
void            init_terminal_data(void);
void            interrogate_terminal(void);
void	        default_term_mode(void);
void	        raw_term_mode(void);
int		tc_putchar(int c);
void	cursor_to_left(t_line *line);
void	cursor_to_right(t_line *line);
int		is_spec(char c);
int		is_not_spec(char c);
void	left_word(t_line *line);
void	right_word(t_line *line);
void	cursor_to_home(t_line *line);
void	cursor_to_end(t_line *line);
void	up_row(t_line *line);
void	down_row(t_line *line);
void	get_cursor_start_pos(t_line *line);
void	set_curpos(t_line *line);
int		ft_getwinsz(t_winsz *winsz);
void	insert_char(t_line *line, int key);
void	delete_char(t_line *line, int key);
int		match_key_curse(char *str);
void	match_move(int key, t_line *line);
void	match_ctrl(int key, t_line *line);
void 	clear_screen_(t_line *line);
void	exit_pgm(t_line *line);

#endif
