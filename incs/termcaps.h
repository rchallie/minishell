/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcap.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 18:06:10 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/20 18:06:17 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TERMCAPS_H
# define TERMCAPS_H
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <term.h>
# include <curses.h>
# include <termios.h>
# include <sys/ioctl.h>
# include "../srcs/libft/libft.h"
# include "./minishell.h"

# define MAX_CMD_LEN 4096
# define MAX_KEY_LEN 4
# define SUCCESS 1
# define ERROR 0

# define KEY_CODE_UP "\x1b\x5b\x41\0"
# define KEY_CODE_DO "\x1b\x5b\x42\0"
# define KEY_CODE_RI "\x1b\x5b\x43\0"
# define KEY_CODE_LE "\x1b\x5b\x44\0"

struct	s_keymatch
{
	char	*key_code;
	int		key_ret;
}       t_key_match;

typedef struct	s_dlist
{
	void			*content;
	size_t			content_size;
	struct s_dlist	*next;
	struct s_dlist	*prev;
}				t_dlist;

typedef struct	s_winsz
{
	size_t	row;
	size_t	col;
}				t_winsz;

typedef struct	s_line
{
	t_dlist	*hist;
	size_t	cursor;
	size_t	length;
	size_t	hist_depth;
	size_t	hist_size;
	t_winsz	winsz;
	t_winsz start;
	char	cmd[MAX_CMD_LEN];
	char	**treated;
}				t_line;

struct	s_keymove
{
	int		key;
	void	(*funct)(t_line *line);
}		t_keymove;

void    init_terminal_data(void);
void    interrogate_terminal(void);
void	default_term_mode(void);
void	raw_term_mode(void);

#endif