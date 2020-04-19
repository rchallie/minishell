/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:54:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/19 16:03:11 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		error_command(char *cmd, t_minishell *ms)
{
	ft_printf("minishell: %s: command not found\n", cmd);
	ms->last_cmd_rtn = 127;
	return (ERROR);
}

int		error_identifier(char *cmd, const char *identifier, char *error)
{
	ft_printf("1 minishell: %s: `%s': %s\n", cmd, identifier, error);
	return (ERROR);
}

int		error_path(const char *cmd, const char *path, int errnum)
{
	ft_printf("2 minishell: %s: %s: %s\n", cmd, path, strerror(errnum));
	return (ERROR);
}