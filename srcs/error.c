/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:54:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/27 11:47:07 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int		error_command(char *cmd, t_minishell *ms)
{
	ft_printf("minishell: %s: command not found\n", cmd);
	ms->last_cmd_rtn = 127;
	return (ERROR);
}

// ERROR NBR TMP a virer.
int		error_identifier(char *msg, const char *identifier)
{
	if (!ft_strcmp(identifier, "\n"))
		ft_printf("minishell: %s `%s'\n", msg, "newline");
	else
		ft_printf("minishell: %s `%s'\n", msg, identifier);
	return (ERROR);
}

int		error_path(const char *cmd, const char *path, int errnum)
{
	ft_printf("minishell: %s: %s: %s\n", cmd, path, strerror(errnum));
	return (ERROR);
}