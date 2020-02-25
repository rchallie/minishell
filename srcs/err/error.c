/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:54:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/24 16:52:40 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		error_command(char *cmd)
{
	ft_printf("minishell: %s: command not found\n", cmd);
	return (ERROR);
}

int		error_identifier(char *cmd, const char *identifier, char *error)
{
	ft_printf("minishell: %s: `%s': %s\n", cmd, identifier, error);
	return (ERROR);
}

int		error_path(const char *cmd, const char *path, int errnum)
{
	ft_printf("minishell: %s: %s: %s\n", cmd, path, strerror(errnum));
	return (ERROR);
}