/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:54:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/20 16:40:07 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

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