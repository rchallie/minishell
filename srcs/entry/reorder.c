/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 10:18:57 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/26 23:36:53 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
**	Function: reorder_args
**	--------------------
**		Reorder arguments.
**
**		(int *)	i	: A pointer to the reorder cursor.
*/

static void	reorder_args(
	int *i,
	char ***cmd,
	int **seq
)
{
	int		x;
	char	*save_value_char;
	int		save_value_int;

	x = *i;
	while (x > 0 && (*seq)[x - 1] && (*seq)[x - 1] != 2)
	{
		save_value_char = (*cmd)[x - 1];
		save_value_int = (*seq)[x - 1];
		(*seq)[x - 1] = (*seq)[x];
		(*cmd)[x - 1] = (*cmd)[x];
		(*seq)[x] = save_value_int;
		(*cmd)[x] = save_value_char;
		x--;
	}
	*i = x;
}

/*
** Function: reorder_seq
** ------------
** 		Reorder the sequence of token and treated
**		array.
**
**		returns: return SUCCESS
*/

int			reorder_sequence(char ***cmd, int **seq)
{
	int i;

	i = 0;
	while ((*cmd)[i])
	{
		if ((*seq)[i] == 2)
			reorder_args(&i, cmd, seq);
		i++;
	}
	return (SUCCESS);
}
