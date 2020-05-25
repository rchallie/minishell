/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 16:42:18 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/25 16:00:48 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
**	Function: is_cmd
**	--------------------
**		Check if a word is looks like a command.
**
**		(char *)	cmd : the word.
**
**		returns:	return !0 : a number corresponding to the command.
**					return -1 : the word is not a command.
*/

int		is_cmd(
	char *cmd
)
{
	if (!ft_strcmp(cmd, "echo"))
		return (0);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (2);
	if (!ft_strcmp(cmd, "env"))
		return (3);
	if (!ft_strcmp(cmd, "cd"))
		return (4);
	if (!ft_strcmp(cmd, "export"))
		return (5);
	if (!ft_strcmp(cmd, "unset"))
		return (6);
	return (-1);
}
