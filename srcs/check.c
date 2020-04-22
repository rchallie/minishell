/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 13:31:08 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/21 19:14:47 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int is_char_spec(char *s)
{
	if (!ft_strcmp(s, ">") || !ft_strcmp(s, "<") || !ft_strcmp(s, ">>")
		|| !ft_strcmp(s, "|") || !ft_strcmp(s, ";"))
		return (SUCCESS);
	return (ERROR);
}

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

int		is_special_token(char *to_test)
{
	if (!ft_strcmp(to_test, ">") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, ">>") && ft_strlen(to_test) == 2)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, "<") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, "|") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, ";") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	else if (!ft_strcmp(to_test, "\n") && ft_strlen(to_test) == 1)
		return (SUCCESS);
	return (ERROR);
}