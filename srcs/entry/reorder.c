/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 10:18:57 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/13 16:31:14 by excalibur        ###   ########.fr       */
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
	save_value_char = ms.treated[save_x];
	save_value_int = ms.sequence[save_x];
	while (save_x > save_i)
	{
		ms.treated[save_x] = ms.treated[save_x - 1];
		ms.sequence[save_x] = ms.sequence[save_x - 1];
		save_x--;
	}
	ms.sequence[save_x] = save_value_int;
	ms.treated[save_x] = save_value_char;
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
	while (ms.treated[x] && ms.sequence[x] != 9)
	{
		if ((ms.sequence[x] == 0) ||
			(ms.sequence[x] == 6 || ms.sequence[x] == 7))
			break ;
		x++;
	}
	if (ms.sequence[x] == 0 && x != *i)
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
	while (x > 0 && ms.sequence[x - 1] && ms.sequence[x - 1] != 2)
	{
		save_value_char = ms.treated[x - 1];
		save_value_int = ms.sequence[x - 1];
		ms.sequence[x - 1] = ms.sequence[x];
		ms.treated[x - 1] = ms.treated[x];
		ms.sequence[x] = save_value_int;
		ms.treated[x] = save_value_char;
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
	while (ms.treated[i])
	{
		if (ms.sequence[i] == 3 || ms.sequence[i] == 4
			|| ms.sequence[i] == 5)
			reorder_redir(&i);
		else if (ms.sequence[i] == 2)
			reorder_args(&i);
		i++;
	}
	return (SUCCESS);
}
