/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 10:18:57 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/19 19:01:56 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
**	Function: reorder_redir_move
**	--------------------
**		Move the redirection on sequence and treated array.
**
**		(int *)	i	: A pointer to the reorder cursor.
**		(int)	x	: A cursor to the posion of token to move.
*/

static void	reorder_redir_move(
	int *i,
	int x
)
{
	int		save_i;
	int		save_x;
	char	*save_value_char;
	int		save_value_int;

	save_x = x;
	save_i = *i;
	save_value_char = g_ms.treated[save_x];
	save_value_int = g_ms.sequence[save_x];
	while (save_x > save_i)
	{
		g_ms.treated[save_x] = g_ms.treated[save_x - 1];
		g_ms.sequence[save_x] = g_ms.sequence[save_x - 1];
		save_x--;
	}
	g_ms.sequence[save_x] = save_value_int;
	g_ms.treated[save_x] = save_value_char;
}

/*
**	Function: reorder_redir
**	--------------------
**		Reorder the redirections (placed at the end).
**
**		(int *)	i	: A pointer to the reorder cursor.
*/

static void	reorder_redir(
	int *i
)
{
	int x;

	x = *i;
	while (g_ms.treated[x] && g_ms.sequence[x] != 9)
	{
		if ((g_ms.sequence[x] == 0) ||
			(g_ms.sequence[x] == 6 || g_ms.sequence[x] == 7))
			break ;
		x++;
	}
	if (g_ms.sequence[x] == 0 && x != *i)
	{
		reorder_redir_move(i, x);
		*i = 0;
	}
}

/*
**	Function: reorder_args
**	--------------------
**		Reorder arguments.
**
**		(int *)	i	: A pointer to the reorder cursor.
*/

static void	reorder_args(
	int *i
)
{
	int		x;
	char	*save_value_char;
	int		save_value_int;

	x = *i;
	while (x > 0 && g_ms.sequence[x - 1] && g_ms.sequence[x - 1] != 2)
	{
		save_value_char = g_ms.treated[x - 1];
		save_value_int = g_ms.sequence[x - 1];
		g_ms.sequence[x - 1] = g_ms.sequence[x];
		g_ms.treated[x - 1] = g_ms.treated[x];
		g_ms.sequence[x] = save_value_int;
		g_ms.treated[x] = save_value_char;
		x--;
	}
	*i = x;
}

/*
** Function: reorder_sequence
** ------------
** 		Reorder the sequence of token and treated
**		array.
**
**		returns: return SUCCESS
*/

int			reorder_sequence(void)
{
	int i;

	i = 0;
	while (g_ms.treated[i])
	{
		if (g_ms.sequence[i] == 3 || g_ms.sequence[i] == 4
			|| g_ms.sequence[i] == 5)
			reorder_redir(&i);
		else if (g_ms.sequence[i] == 2)
			reorder_args(&i);
		i++;
	}
	return (SUCCESS);
}
