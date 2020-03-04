/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reorder.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/03 10:18:57 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/04 09:25:49 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int reorder_sequence(t_minishell *ms)
{
	int i;
	int x;
	int save_i;
	int save_x;
	char *save_value_char;
	int save_value_int;


	i = 0;
	x = 0;
	save_i = 0;
	save_x = 0;
	save_value_char = NULL;
	save_value_int = 0;

	ft_printf("\n--- PREVIOUS REORDER ---\n");
	int j = 0;
	while (j < get_double_char_tab_len(ms->treated))
	{
		if (ms->sequence[j] == 0)
			printf("{%d} %d | CMD		(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 1)
			printf("{%d} %d | FLAG		(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 2)
			printf("{%d} %d | ARG		(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 3)
			printf("{%d} %d | REDIR \">\"	(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 4)
			printf("{%d} %d | REDIR \">>\"	(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 5)
			printf("{%d} %d | REDIR  \"<\"	(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 6)
			printf("\n{%d} %d | PIPE\n\n", j, ms->sequence[j]);
		else if (ms->sequence[j] == 7)
			printf("{%d} %d | END\n", j, ms->sequence[j]);
		else if (ms->sequence[j] == 8)
			printf("{%d} %d | FILE		(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 9)
			printf("{%d} %d | NEWLINE\n", j, ms->sequence[j]);
		else if (ms->sequence[j] == -1)
			printf("{%d} %d | ERROR\n", j, ms->sequence[j]);
		j++;
	}

	while (ms->treated[i])
	{
		// REDIR
		if (ms->sequence[i] == 3 || ms->sequence[i] == 4 || ms->sequence[i] == 5)
		{
			x = i;
			while (ms->treated[x] && ms->sequence[x] != 9)
			{
				if (ms->sequence[x] == 0)
					break;
				else if (ms->sequence[x] == 6 || ms->sequence[x] == 7)
					break;
				x++;
			}
			if (ms->sequence[x] == 0 && x != i)
			{
				save_x = x;
				save_i = i;
				save_value_char = ms->treated[save_x];
				save_value_int = ms->sequence[save_x];
				while (save_x > save_i)
				{
					ms->treated[save_x] = ms->treated[save_x - 1];
					ms->sequence[save_x] = ms->sequence[save_x - 1];			
					save_x--;
				}
				ms->sequence[save_x] = save_value_int;
				ms->treated[save_x] = save_value_char;
				i = 0;
				// exit(0);
			}
		} // ARG
		else if (ms->sequence[i] == 2)
		{
			x = i;
			while (x > 0 && ms->sequence[x - 1] && ms->sequence[x - 1] != 2)
			{
				save_value_char = ms->treated[x - 1];
				save_value_int = ms->sequence[x - 1];
				ms->sequence[x - 1] = ms->sequence[x];
				ms->treated[x - 1] = ms->treated[x];
				ms->sequence[x] = save_value_int;
				ms->treated[x] = save_value_char;
				x--;
			}
			i = x;
		}
		i++;
	}

	ft_printf("\n--- FINAL REORDER ---\n");
	j = 0;
	while (j < get_double_char_tab_len(ms->treated))
	{
		if (ms->sequence[j] == 0)
			printf("{%d} %d | CMD		(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 1)
			printf("{%d} %d | FLAG		(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 2)
			printf("{%d} %d | ARG		(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 3)
			printf("{%d} %d | REDIR \">\"	(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 4)
			printf("{%d} %d | REDIR \">>\"	(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 5)
			printf("{%d} %d | REDIR  \"<\"	(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 6)
			printf("\n{%d} %d | PIPE\n\n", j, ms->sequence[j]);
		else if (ms->sequence[j] == 7)
			printf("{%d} %d | END\n", j, ms->sequence[j]);
		else if (ms->sequence[j] == 8)
			printf("{%d} %d | FILE		(%s)\n", j, ms->sequence[j], ms->treated[j]);
		else if (ms->sequence[j] == 9)
			printf("{%d} %d | NEWLINE\n", j, ms->sequence[j]);
		else if (ms->sequence[j] == -1)
			printf("{%d} %d | ERROR\n", j, ms->sequence[j]);
		j++;
	}
	ft_printf("\n");
	return (SUCCESS);
}