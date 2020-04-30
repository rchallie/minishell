/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 17:24:07 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/30 14:53:49 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		exit_minishell(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	(void)envp;
	default_term_mode();
	//while(1);
	exit(0);
}
