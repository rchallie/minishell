/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/24 16:42:18 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/25 09:14:02 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		is_cmd(char *cmd)
{
	// check pour absolute path etc
	if (!ft_strcmp(cmd, "cd"))
		return (0); 
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "echo"))
		return (2);
	if (!ft_strcmp(cmd, "export"))
		return (3);
	if (!ft_strcmp(cmd, "unset"))
		return (4);
	if (!ft_strcmp(cmd, "env"))
		return (5);
	if (!ft_strcmp(cmd, "exit"))
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