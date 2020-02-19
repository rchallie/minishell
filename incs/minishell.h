/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 14:02:29 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/19 16:54:25 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define SUCCESS 1
# define ERROR 0

# include "../srcs/libft/libft.h"

int             ft_secure_strlen(char *str);
int             ft_is_whitespace(char c);
int		        sanitize(char *entry, char **treated);

#endif