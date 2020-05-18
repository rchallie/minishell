/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_interr_term.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 19:37:14 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/15 15:35:59 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void	init_terminal_data(void)
{
	char		*termtype;
	int			success;
	static char	term_buffer[2046];

	termtype = getenv("TERM");
	if (termtype == 0)
		ft_putstr("Specify a terminal type with `setenv TERM <yourtype>'.\n");
	success = tgetent(term_buffer, (const char *)termtype);
	if (success < 0)
		ft_putstr("Could not access the termcap data base.\n");
	if (success == 0)
		ft_printf(STDERR_FILENO, "Terminal type `%s' is not defined.\n", termtype);
}
