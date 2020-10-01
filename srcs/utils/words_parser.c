/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   words_parser.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/22 14:40:44 by excalibur         #+#    #+#             */
/*   Updated: 2020/10/01 16:04:33 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int		no_quotes(
	char **entry,
	char **word,
	int *simple_q,
	int *double_q
)
{
	if (**entry == '\\')
	{
		*word = add_char_to_word_free(*word, *(*entry)++);
		*word = add_char_to_word_free(*word, **entry);
		return (1);
	}
	else if (**entry == '\'' || **entry == '\"')
	{
		if (**entry == '\'')
			(*simple_q = 1);
		else
			(*double_q = 1);
		*word = add_char_to_word_free(*word, **entry);
	}
	else
		*word = add_char_to_word_free(*word, **entry);
	return (0);
}

static int		if_quotes(
	char **entry,
	char **word,
	int *simple_q,
	int *double_q
)
{
	if (*simple_q == 1)
	{
		if (**entry == '\\')
		{
			*word = add_char_to_word_free(*word, *(*entry)++);
			*word = add_char_to_word_free(*word, **entry);
			return (1);
		}
		else if (**entry == '\'')
			*simple_q = 0;
		*word = add_char_to_word_free(*word, **entry);
	}
	else if (*double_q == 1)
	{
		if (**entry == '\\')
		{
			*word = add_char_to_word_free(*word, *(*entry)++);
			*word = add_char_to_word_free(*word, **entry);
			return (1);
		}
		else if (**entry == '\"')
			*double_q = 0;
		*word = add_char_to_word_free(*word, **entry);
	}
	return (0);
}

static int		normal_char(
	char **startword,
	char **word,
	int q_mask
)
{
	char c;

	c = **startword;
	if ((((**startword == ' ' || **startword == '>' || **startword == '<'
			|| **startword == '|' || **startword == ';'))) && !(q_mask))
	{
		if (*word)
			return (1);
		while (**startword == c)
			*word = add_char_to_word_free(*word, *(*startword)++);
		if ((ft_secure_strlen(*word) > 2 && c == '>')
			|| (ft_secure_strlen(*word) > 1 && c != '>'))
		{
			ft_printf(2, "minishell: %s `%c%c'\n",
				"syntax error near unexpected token", c, c);
			return (-1);
		}
		return (1);
	}
	return (0);
}

int				get_word(
	char *startword,
	char **word,
	int *c_cnt
)
{
	int		sim_q;
	int		dou_q;
	char	*save_startword;
	int		rtn;

	sim_q = 0;
	dou_q = 0;
	save_startword = startword;
	rtn = 0;
	while (*startword)
	{
		if ((rtn = normal_char(&startword, word, sim_q | dou_q)) == -1)
			return (-1);
		else if (rtn == 1)
			break ;
		(sim_q == 0 && dou_q == 0) ? no_quotes(&startword, word, &sim_q, &dou_q)
			: if_quotes(&startword, word, &sim_q, &dou_q);
		(*startword) ? startword++ : 0;
		if (*startword == '\0' && (sim_q || dou_q) && isatty(0))
			return ((ft_printf(2, "minishell: multiligne\n") > 0) ? 0 : -1);
	}
	*c_cnt = startword - save_startword;
	return (SUCCESS);
}
