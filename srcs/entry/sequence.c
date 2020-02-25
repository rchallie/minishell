/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sequence.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 14:46:46 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/24 17:21:27 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int	is_special(char *to_test, int *sequ, int i)
{
	if (!ft_strcmp(to_test, ">") && ft_strlen(to_test) == 1)
	{
		sequ[i] = 3;
		return (SUCCESS);
	}
	else if (!ft_strcmp(to_test, ">>") && ft_strlen(to_test) == 2)
	{
		sequ[i] = 4;
		return (SUCCESS);
	}
	else if (!ft_strcmp(to_test, "<") && ft_strlen(to_test) == 1)
	{
		sequ[i] = 5;
		return (SUCCESS);
	}
	else if (!ft_strcmp(to_test, "|") && ft_strlen(to_test) == 1)
	{
		sequ[i] = 6;
		return (SUCCESS);
	}
	else if (!ft_strcmp(to_test, ";") && ft_strlen(to_test) == 1)
	{
		sequ[i] = 7;
		return (SUCCESS);
	}
	return (ERROR);
}

int get_sequence(char **treated, int **sequence)
{
	int i;
	int nbr_args;
	int num_sequ;
	int *sequ;

	(void)sequence;
	
	i = 0;
	nbr_args = get_double_char_tab_len(treated);
	num_sequ = 0;
	sequ = NULL;
	if (!treated && !*treated)
		return (SUCCESS);
	if (!(sequ = (int *)malloc(sizeof(int) * (nbr_args + 1))))
		return (ERROR);
	ft_bzero(sequ, sizeof(int) * (nbr_args + 1));
	while (treated[i])
	{
		if (num_sequ == 0 || (i != 0 && (sequ[i - 1] == 6 || sequ[i - 1] == 7)))
		{
			sequ[i] = 0;
			num_sequ = 2;
		}
		else if (treated[i][0] == '-')
			sequ[i] = 1;
		else if (!is_special(treated[i], sequ, i) && num_sequ == 2)
			sequ[i] = 2;
		i++;
	}
	// int j = 0;
	// while (j < i)
	// {
	// 	printf("{%d} %d\n", j, sequ[j]);
	// 	j++;
	// }
	*sequence = sequ;
	return (SUCCESS);
}