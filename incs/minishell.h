/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:02:29 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/18 16:14:23 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define TREAT 2
# define SUCCESS 0x1
# define ERROR 0
# define NEED_QUOTE -1
# define NEED_DQUOTE -2

# define ERROR_NEAR_UNEXPECTED_AT_NEXT_POS 0x8
# define ERROR_NEAR_UNEXPECTED_AT_POS 0x9
# define ERROR_SIGINT 0xA
# define ERROR_SIGQUIT 0xB
# define ERROR_NO_CURRENT_WORK_DIR 0xC

# include "../srcs/libft/libft.h"
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>
# include <term.h>
# include <curses.h>
# include <termios.h>
# include <dirent.h>
# include <sys/ioctl.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <errno.h>
# include <signal.h>

# define HISTORY_PATH ".save_history"
# define MAX_CMD_LEN 4096
# define MAX_KEY_LEN 7

# define KEY_CODE_UP "\x1b\x5b\x41\0"
# define KEY_CODE_DO "\x1b\x5b\x42\0"
# define KEY_CODE_RI "\x1b\x5b\x43\0"
# define KEY_CODE_LE "\x1b\x5b\x44\0"
# define KEY_CODE_CTRL_C "\x03"
# define KEY_CODE_CTRL_D "\x04"
# define KEY_CODE_CTRL_L "\x0c"
# define KEY_CODE_HOME "\x1b\x5b\x48"
# define KEY_CODE_END "\x1b\x5b\x46"
# define KEY_CODE_CTRL_LE "\x1b\x5b\x31\x3b\x35\x44"
# define KEY_CODE_CTRL_RI "\x1b\x5b\x31\x3b\x35\x43"
# define KEY_CODE_CTRL_UP "\x1b\x5b\x31\x3b\x35\x41"
# define KEY_CODE_CTRL_DO "\x1b\x5b\x31\x3b\x35\x42"
# define KEY_CODE_SHIFT_LE "\x1b\x5b\x31\x3b\x32\x44"
# define KEY_CODE_SHIFT_RI "\x1b\x5b\x31\x3b\x32\x43"

# define KEY_CTRL_LE -10
# define KEY_CTRL_RI -11
# define KEY_CTRL_UP -12
# define KEY_CTRL_DO -13
# define KEY_CTRL_D -20
# define KEY_CTRL_L -21
# define KEY_CTRL_C -22

typedef struct		s_exec
{
	char			*exec;
	char			*exec_path;
	char			*env_path;
	char			**path_list;
	char			**argv;
	int				save_seq_cursor;
}					t_exec;

typedef struct		s_minishell
{
	char			*entry;
	char			**treated;
	int				*sequence;
	int				seq_cursor;
	int				iscmdret;
	int				isexecret;
	int				treated_len;
	int				has_spec_uf;
	int				nbr_pipe;
	int				has_pipe;
	int				cursor;
	int				last_cmd_rtn;
	char			*pwd;
}					t_minishell;

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
	size_t			cursor;
	int				cursor_highl;
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

typedef struct		s_highlight
{
	char			*beg;
	char			*hlight;
	char			*end;
}					t_highlight;

typedef struct		s_keyhist
{
	int				key;
	void			(*funct)(t_line *line, t_dlist **hist);
}					t_keyhist;

typedef struct		s_keymove_hl
{
	int				key;
	void			(*funct)(int key, t_line *line);
}					t_keymove_hl;

typedef struct		s_keymove_ms
{
	int				key;
	void			(*funct)(int *key, t_line *line);
}					t_keymove_ms;

typedef struct		s_args
{
	struct s_args	*next;
	void			*content;
}					t_args;

char				**g_envp;
char				**g_export_vars;
char				*g_pwd;
t_minishell			g_ms;

/*
** _____ Signal catchers _____
*/

void				sigcatcher_init();
void				sigint_catcher();
void				sigquit_catcher();

/*
** _____ Double char tab _____
*/

void				free_double_char_tab(char **tab_to_free);
int					get_double_char_tab_len(char **tabl);
int					dup_double_char_tab(char **src, char ***new_tab);
int					double_char_tab_contain(char *name, char **from);
char				**double_tab_bubble_sort(char ***sort_me);
int					double_char_tab_remove(char **to_remove, char ***array);
int					add_word_to_tab(char *word, char ***treated);
char				**new_double_char_tab_init(size_t double_tab_size, char *str_init);

/*
** _____ Char array _____
*/

char				*add_char_to_word(char *word, char c);
char				*add_char_to_word_free(char *word, char c);

/*
** _____ Environment _____
*/

char				*get_env_var_by_name(char *name);
int					add_var_to_env(char *var);

/*
** _____ Utils _____
*/

int					get_next_line(int fd, char **line);
int					ft_printf(int fd, const char *str, ...);
int					ft_secure_strlen(const char *str);
int					ft_is_whitespace(char c);
int					get_word(char *startword, char **entry_addr, char **word);

/*
** _____ Pwd _____
*/

void				get_shell_pwd(char **pwd);
int					get_pwd(char **pwd);
int					get_pwd_short(char **pwd);
int					get_pwd_short_from(char **pwd);
/*
** _____ Commands _____
*/

int					is_exec(char **cmd, int *seq);
int					export_(int argc, char **argv, char **envp);
int					cd(int argc, char **argv, char **envp);
int					print_work_dir(int argc, char **argv, char **envp);
int					env(int argc, char **argv, char **envp);
int					echo_(int argc, char **argv, char **envp);
int					exit_minishell(int argc, char **argv, char **envp);
int					unset(int argc, char **argv, char **envp);

/*
** _____ Command line treatment _____
*/

int					get_sequence(char **treated, int **sequence);
int					sanitize(char *entry, char ***treated);
int					reorder_sequence(char ***cmd, int **seq);
int					has_redir(char **cmd, int *seq, int *fdin, int *fdout);
int					has_redir_output(int redir_type, int cursor, int fd, char **cmd, int *seq);
int					has_redir_input(int redir_type, int cursor, int fd, char **cmd, int *seq);
void				cmd_has_pipe(char **cmd, int *seq);
void				cmd_no_pipe(char **cmd, int *seq);
int					treat_command(char **cmd, int *seq);

/*
** _____ Errors _____
*/

int					error_path(const char *cmd, const char *path,
						int errnum);
int					error_identifier(char *msg, const char *identifier);
int					error_command(char *cmd);
int					error_file(char *file_name, int errnum);
int					error_unidentified(char *msg, const char *unidentified);

/*
** _____ Checkers _____
*/

int					is_cmd(char *cmd);
int					is_char_spec(char *s);
int					is_special_token(char *to_test);
int					remove_var_env(char *var_name);

/*
** _____ Termcaps _____
*/

void				init_terminal_data(void);
void				interrogate_terminal(void);
void				default_term_mode(void);
void				raw_term_mode(void);
void				line_edition(void);
void				clear_term(void);
void				put_beg(void);
int					print_prompt(void);
int					tc_putchar(int c);
void				get_cursor_start_pos(t_line *line);
int					ft_getwinsz(t_winsz *winsz);
void				set_curpos(t_line *line);
char				*edit_line(void);
void				cursor_to_left(t_line *line);
void				cursor_to_right(t_line *line);
int					is_spec(char c);
int					is_not_spec(char c);
void				left_word(t_line *line);
void				right_word(t_line *line);
void				cursor_to_home(t_line *line);
void				cursor_to_end(t_line *line);
void				up_row(t_line *line);
void				down_row(t_line *line);
void				insert_char(t_line *line, int key);
void				delete_char(t_line *line, int key);
void				set_line(int save, t_line *line);
int					match_key_curse(char *str);
void				find_match(int *key, t_line *line);
void				match_move(int key, t_line *line);
void				highlight(int key, t_line *line);
void				match_highlight(int key, t_line *line);
void				match_hist(int key, t_line *line);
void				match_ctrl(int *key, t_line *line);
void				clear_screen_(int *key, t_line *line);
void				exit_pgm(int *key, t_line *line);
t_dlist				*get_history(void);
void				append_history(char *new_hist);
void				old_history(t_line *line, t_dlist **hist);
void				new_history(t_line *line, t_dlist **hist);
int					ft_dlst_size(t_dlist *hist);
void				ft_dlst_del(t_dlist **list);
void				ft_dlst_remove_link(t_dlist **head);
void				ft_dlst_add(t_dlist **head, t_dlist *new);
t_dlist				*ft_dlst_new(void const *content, int content_size);
void				launch_(t_line line);
void				welcome_to_minishell(char *launching, t_line line,
					char *str, int nb);
void				high_left(t_line *line);
void				high_right(t_line *line);
void				reset_line(t_line *line);

#endif
