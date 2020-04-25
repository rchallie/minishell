/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 16:42:18 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/22 16:00:55 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		is_cmd(char *cmd)
{
	// check pour absolute path etc
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

// ◦ echo et l’option ’-n’
// ◦ cd uniquement avec un chemin absolu ou relatif
// ◦ pwd sans aucune option
// ◦ export sans aucune option
// ◦ unset sans aucune option

// ◦ env sans aucune option ni argument
// ◦ exit sans aucune option