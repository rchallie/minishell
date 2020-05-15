/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:54:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/13 15:45:09 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int		error_command(char *cmd)
{
	ft_printf("minishell: %s: command not found\n", cmd);
	ms.last_cmd_rtn = 127;
	return (1);
}

int		error_identifier(char *msg, const char *identifier)
{
	if (!ft_strcmp(identifier, "\n"))
		ft_printf("minishell: %s `%s'\n", msg, "newline");
	else
		ft_printf("minishell: %s `%s'\n", msg, identifier);
	ms.last_cmd_rtn = 2;
	return (ERROR);
}

int		error_unidentified(char *msg, const char *unidentified)
{
	ft_printf("minishell: %s: %s not set\n", msg, unidentified);
	return (1);
}

int		error_path(const char *cmd, const char *path, int errnum)
{
	ft_printf("minishell: %s: %s: %s\n", cmd, path, strerror(errnum));
	ms.last_cmd_rtn = errnum;
	return (1);
}
