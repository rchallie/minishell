/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:02:29 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/20 17:06:06 by rchallie         ###   ########.fr       */
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
# include <sys/types.h>
# include <dirent.h>

typedef struct s_args
{
	struct s_args	*next;
	void			*content;
}	t_args;

int		        ft_printf(const char *str, ...);
int             ft_secure_strlen(const char *str);
int             ft_is_whitespace(char c);
int		        sanitize(char *entry, char **treated);
int				get_pwd(char **pwd);
int				get_pwd_short(char **pwd);
int				get_word(char *entry, char **word);

char			*add_char_to_word(char *word, char c);

int				cd(const char *path);
int				print_work_dir(void);
int				env(char **envp);
int				export(t_args *args);

int				error_path(const char *cmd, const char *path, int errnum);
int				error_identifier(char *cmd, const char *identifier, char *error);


#endif