/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sequence_tuning.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/27 10:18:37 by excalibur         #+#    #+#             */
/*   Updated: 2020/09/27 10:41:50 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
**	Function: tuning_home
**	--------------------
**		Modify '~' to home directory in and create new word.
**
**		(char *)	word : the word to treat.
**
**		returns:	return : a word with treated '~'.
*/

static char		*tuning_home(char *word)
{
	char *home;
	char *tmp_word;
	char *save_word;

	save_word = word;
	home = get_env_var_by_name("HOME");
	if (ft_secure_strlen(home) != 0)
		word = ft_strjoin(home, (word + 1));
	else
	{
		tmp_word = ft_strjoin("/Users/", get_env_var_by_name("USER"));
		word = ft_strjoin(tmp_word, (word + 1));
		free(tmp_word);
	}
	(save_word) ? free(save_word) : 0;
	(home) ? free(home) : 0;
	return (word);
}

static void		tuning_quotes(
	char **str,
	int *simple_q,
	int *double_q,
	char **new_str
)
{
	if (**str == '\\' && *simple_q == 0)
	{
		if (*double_q == 1 && *(*(str + 1)) && *(*str + 1) != '\\'
			&& *(*str + 1) != '$' && *(*str + 1) != '\"')
			*new_str = add_char_to_word_free(*new_str, **str);
		(*str)++;
		*new_str = add_char_to_word_free(*new_str, **str);
	}
	else if (**str == '\"' && *double_q == 0 && *simple_q != 1)
		*double_q = 1;
	else if (**str == '\"' && *double_q == 1)
		*double_q = 0;
	else if (**str == '\'' && *simple_q == 0 && *double_q != 1)
		*simple_q = 1;
	else if (**str == '\'' && *simple_q == 1)
		*simple_q = 0;
	else
		*new_str = add_char_to_word_free(*new_str, **str);
}

static void		tuning_treated(
	char **str
)
{
	int		simple_q;
	int		double_q;
	char	*new_str;
	char	*save;

	new_str = NULL;
	simple_q = 0;
	double_q = 0;
	save = *str;
	while (**str)
	{
		tuning_quotes(str, &simple_q, &double_q, &new_str);
		(*str)++;
	}
	if (new_str == NULL)
		new_str = ft_strdup("");
	*str = new_str;
	(save) ? free(save) : 0;
	return ;
}

int				tuning_sequence(
	char **treated,
	int *sequ,
	int i
)
{
	if (sequ[i] == 0 || sequ[i] == 2 || sequ[i] == 8)
	{
		if (treated[i] && treated[i][0] == '~'
			&& (!treated[i][1] || treated[i][1] == '/'))
			treated[i] = tuning_home(treated[i]);
		tuning_treated(&(treated[i]));
	}
	else if (sequ[i] == 7)
		treated[i][0] = '\n';
	return (SUCCESS);
}
