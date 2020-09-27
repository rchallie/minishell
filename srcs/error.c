/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:54:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/27 21:27:18 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int		error_command(char *cmd)
{
	ft_printf(2, "minishell: %s: command not found\n", cmd);
	g_ms.last_cmd_rtn = 127;
	return (1);
}

int		error_identifier(char *msg, const char *identifier)
{
	if (!ft_strcmp(identifier, "\n"))
		ft_printf(2, "minishell: %s `%s'\n", msg, "newline");
	else
		ft_printf(2, "minishell: %s `%s'\n", msg, identifier);
	g_ms.last_cmd_rtn = 2;
	return (ERROR);
}

int		error_unidentified(char *msg, const char *unidentified)
{
	ft_printf(2, "minishell: %s: %s not set\n", msg, unidentified);
	g_ms.last_cmd_rtn = 1;
	return (1);
}

int		error_path(const char *cmd, const char *path, int errnum)
{
	ft_printf(2, "minishell: %s: %s: %s\n",
		cmd, path, strerror(errnum));
	g_ms.last_cmd_rtn = errnum;
	return (1);
}

int		error_file(char *file_name, int errnum)
{
	ft_printf(2, "minishell: %s: %s\n",
		file_name, strerror(errnum));
	g_ms.last_cmd_rtn = 1;
	return (-1);
}
