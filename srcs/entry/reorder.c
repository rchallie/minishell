/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 10:18:57 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/29 16:25:18 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
**	Function: reorder_redir_move
**	--------------------
**		Move the redirection on seq and treated array.
**
**		(int *)	i	: A pointer to the reorder cursor.
**		(int)	x	: A cursor to the posion of token to move.
*/

static void	reorder_redir_move(
	int *i,
	int x,
	char ***cmd,
	int **seq
)
{
	int		save_i;
	int		save_x;
	char	*save_value_char;
	int		save_value_int;

	save_x = x;
	save_i = *i;
	save_value_char = (*cmd)[save_x];
	save_value_int = (*seq)[save_x];
	while (save_x > save_i)
	{
		(*cmd)[save_x] = (*cmd)[save_x - 1];
		(*seq)[save_x] = (*seq)[save_x - 1];
		save_x--;
	}
	(*seq)[save_x] = save_value_int;
	(*cmd)[save_x] = save_value_char;
}

/*
**	Function: reorder_redir
**	--------------------
**		Reorder the redirections (placed at the end).
**
**		(int *)	i	: A pointer to the reorder cursor.
*/

static void	reorder_redir(
	int *i,
	char ***cmd,
	int **seq
)
{
	int x;

	x = *i;
	while ((*cmd)[x] && (*seq)[x] != 9)
	{
		if ((*seq)[x] == 0 ||
			((*seq)[x] == 6 || (*seq)[x] == 7))
			break ;
		x++;
	}
	if ((*seq)[x] == 0 && x != *i)
	{
		reorder_redir_move(i, x, cmd, seq);
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

	ft_printf(1, "\n--- PREVIOUS REORDER ---\n");
	int j = 0;
	char **d_cmd = *cmd;
	while (j < get_double_char_tab_len(d_cmd))
	{
		ft_printf(1, "AA\n");
		ft_printf(1, "*seq[j] = %d\n", (*seq)[j]);
		if ((*seq)[j] == 0)
			ft_printf(1, "{%d} %d | CMD		(%s)\n", j, (*seq)[j], (d_cmd[j]));
		else if ((*seq)[j] == 1)
			ft_printf(1, "{%d} %d | FLAG		(%s)\n", j, (*seq)[j], (d_cmd[j]));
		else if ((*seq)[j] == 2)
			ft_printf(1, "{%d} %d | ARG		(%s)\n", j, (*seq)[j], (d_cmd[j]));
		else if ((*seq)[j] == 3)
			ft_printf(1, "{%d} %d | REDIR \">\"	(%s)\n", j, (*seq)[j], (d_cmd[j]));
		else if ((*seq)[j] == 4)
			ft_printf(1, "{%d} %d | REDIR \">>\"	(%s)\n", j, (*seq)[j], (d_cmd[j]));
		else if ((*seq)[j] == 5)
			ft_printf(1, "{%d} %d | REDIR  \"<\"	(%s)\n", j, (*seq)[j], (d_cmd[j]));
		else if ((*seq)[j] == 6)
			ft_printf(1, "\n{%d} %d | PIPE\n\n", j, (*seq)[j]);
		else if ((*seq)[j] == 7)
			ft_printf(1, "{%d} %d | END\n", j, (*seq)[j]);
		else if ((*seq)[j] == 8)
			ft_printf(1, "{%d} %d | FILE		(%s)\n", j, (*seq)[j], (d_cmd[j]));
		else if ((*seq)[j] == 9)
			ft_printf(1, "{%d} %d | NEWLINE\n", j, (*seq)[j]);
		else if ((*seq)[j] == -1)
			ft_printf(1, "{%d} %d | ERROR\n", j, (*seq)[j]);
		j++;
	}
	
	i = 0;
	ft_printf(1, "CMD[i] start = %d\n", *(*(cmd[i])));
	while ((*cmd)[i])
	{
		ft_printf(1, "A\n");
		if ((*seq)[i] == 3 || (*seq)[i] == 4
			|| (*seq)[i] == 5)
			reorder_redir(&i, cmd, seq);
		else if ((*seq)[i] == 2)
			reorder_args(&i, cmd, seq);
		i++;
	}
	return (SUCCESS);
}
