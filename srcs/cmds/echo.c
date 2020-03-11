/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 10:23:19 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/11 14:39:16 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
** Function: echo_
** ------------
** 		Write arguments to the standard output.
**		Display the ARGs on the standard output followed by a newline
**
**		(t_minishell *)ms : minishell global variables
**
**		returns: return 1
*/

int				echo_(t_minishell *ms)
{
	int		cursor;
	int		len;
	int		has_opt;

	cursor = ms->seq_cursor + 1;
	has_opt = (!ft_strcmp(ms->treated[cursor], "-n")) ? 1 : 0;
	cursor += has_opt;
	while (ms->sequence[cursor] == 2)
	{
		len = 0;
		while (len < (int)ft_strlen(ms->treated[cursor]))
			write(1, &ms->treated[cursor][len++], 1);
		if (ms->sequence[cursor + 1] == 2)
			write(1, " ", 1);
		cursor++;
	}
	if (!has_opt)
		write(1, "\n", 1);
	return (SUCCESS);
}
