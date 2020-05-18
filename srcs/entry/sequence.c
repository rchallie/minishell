/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sequence.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/21 14:46:46 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/13 18:16:17 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
** Function : is_special
** -------------------------
**		Init the value at i in sequ in function of passed string (to_test)
**
**		(char *)to_test : char to compare with special things
**		(int *) seq		: sequence
**		(int)	i		: index in sequ
**
**		returns:	return 1 : if to_test is a special thing
**					return 0 : if not
*/

static int		is_special(char *to_test, int *sequ, int i)
{
	if (!ft_strcmp(to_test, ">") && ft_strlen(to_test) == 1)
		sequ[i] = 3;
	else if (!ft_strcmp(to_test, ">>") && ft_strlen(to_test) == 2)
		sequ[i] = 4;
	else if (!ft_strcmp(to_test, "<") && ft_strlen(to_test) == 1)
		sequ[i] = 5;
	else if (!ft_strcmp(to_test, "|") && ft_strlen(to_test) == 1)
		sequ[i] = 6;
	else if (!ft_strcmp(to_test, ";") && ft_strlen(to_test) == 1)
		sequ[i] = 7;
	else if (!ft_strcmp(to_test, "\n") && ft_strlen(to_test) == 1)
		sequ[i] = 9;
	if (sequ[i] == 3 || sequ[i] == 4 || sequ[i] == 5 || sequ[i] == 6
		|| sequ[i] == 7 || sequ[i] == 9)
		return (SUCCESS);
	return (ERROR);
}

/*
** Function : seq_for_special
** -------------------------
**		Sequence the next thing if the previous was a special thing
**
**		(char **)treated : treated entry
**		(int *)  seq	 : sequence
**		(int *)	 has_cmd : a flag if the cmd word was set, so has_cmd = 1 else 0
**		(int)	 i		 : index in sequ
**
**		returns:	return 0 : Everithing is okay.
**					return 1 : An error appear.
*/

static int		seq_for_special(char **treated, int *sequ, int *has_cmd, int *i)
{
	if (sequ[*i] == 3 || sequ[*i] == 4 || sequ[*i] == 5)
	{
		*i += 1;
		if (treated[*i] && !is_special(treated[*i], sequ, *i))
			sequ[*i] = 8;
		else
			return (error_identifier("syntax error near unexpected token",
				treated[*i]));
	}
	else if (sequ[*i] == 7 || sequ[*i] == 6)
	{
		if (*has_cmd == 0)
			return (error_identifier("syntax error near unexpected token",
				treated[*i]));
		else
			*has_cmd = 0;
	}
	return (SUCCESS);
}

/*
** Function : seq_treated_tab
** -------------------------
**		Sequence the treated entry tab
**
**		(char **)treated : treated entry
**		(int *)  seq	 : sequence
**
**		returns:	return 0x0 : ERROR
**					return 0x1 : SUCCESS
*/

static int		seq_treated_tab(char **treated, int *sequ)
{
	int	has_cmd;
	int i;
	int ret_spe;

	i = 0;
	has_cmd = 0;
	ret_spe = 0;
	while (treated[i])
	{
		if (is_special(treated[i], sequ, i))
		{
			if (((ret_spe =
				seq_for_special(treated, sequ, &has_cmd, &i)) != SUCCESS))
				return (ret_spe);
		}
		else if (has_cmd == 0)
		{
			sequ[i] = 0;
			has_cmd = 1;
		}
		else
			sequ[i] = 2;
		i++;
	}
	return (SUCCESS);
}

/*
** Function : get_sequence
** -------------------------
**		get a sequence (tockenization) of the treated entry tab
**
**		(char **)treated : treated entry
**		(int **)  seq	 : sequence to return
**
**		returns:	return 0x1 : SUCCESS
**					return 0x0 : ERROR
*/

int				get_sequence(char **treated, int **sequence)
{
	int treated_len;
	int *sequ;
	int ret_treat;

	treated_len = get_double_char_tab_len(treated);
	if (!treated && !*treated)
		return (SUCCESS);
	if (!(sequ = (int *)malloc(sizeof(int) * (treated_len + 2))))
		return (ERROR);
	ft_bzero(sequ, sizeof(int) * (treated_len + 2));
	if ((ret_treat = seq_treated_tab(treated, sequ)) == SUCCESS)
		*sequence = sequ;
	return (ret_treat);
}
