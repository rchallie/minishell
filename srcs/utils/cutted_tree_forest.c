/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cutted_tree_forest.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/21 16:10:38 by excalibur         #+#    #+#             */
/*   Updated: 2020/05/05 15:39:40 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int		tree_named_env(char **entry, char **word)
{
	int		rtn;
	char	*env_var_name;

	env_var_name = NULL;
	rtn = 0;
	if (**entry == '$')
	{
		(*entry)++;
		rtn++;
		while (*entry && (ft_isalnum(**entry)
			|| **entry == '?' || **entry == '_'))
		{
			env_var_name = add_char_to_word_free(env_var_name, **entry);
			(*entry)++;
			rtn++;
		}
		if (ft_secure_strlen(env_var_name) == 1 && env_var_name[0] == '?')
			env_var_name = ft_itoa(ms.last_cmd_rtn);
		else
			env_var_name = get_env_var_by_name(env_var_name);
		*word = ft_strjoin(*word, env_var_name);
		*word = add_char_to_word_free(*word, '\0');
	}
	return (rtn);
}

static int		tree_named_backslash(
	char **entry,
	char **word,
	int simple_q,
	int double_q
)
{
	int rtn;

	rtn = 0;
	if (**entry == '\\' && simple_q == 0 && double_q == 0 && *(*entry + 1))
	{
		(*entry)++;
		*word = add_char_to_word_free(*word, **entry);
		rtn++;
	}
	else if (double_q == 1 && **entry == '\\' && *(*entry + 1) == '\"')
	{
		(*entry)++;
		rtn++;
		*word = add_char_to_word_free(*word, **entry);
		(*entry)++;
		rtn++;
	}
	return (rtn);
}

static void		tree_named_quote(
	char *entry,
	char **word,
	int *simple_q,
	int *double_q
)
{
	if (*entry == '\'' && *simple_q == 0 && *double_q == 0)
		*simple_q = 1;
	else if (*entry == '\'' && *double_q == 1)
		*word = add_char_to_word_free(*word, *entry);
	else if (*entry == '\'')
		*simple_q = 0;
	if (*entry == '\"' && *double_q == 0 && *simple_q == 0)
		*double_q = 1;
	else if (*entry == '\"' && *simple_q == 1)
		*word = add_char_to_word_free(*word, *entry);
	else if (*entry == '\"')
		*double_q = 0;
}

static int		tree_named_last(char **entry, char **word)
{
	if (**entry != '\'' && **entry != '\"')
		*word = add_char_to_word_free(*word, **entry);
	if (!word)
		return (ERROR);
	if (*entry)
		(*entry)++;
	return (SUCCESS);
}

int				get_word(char *startword, char **entry_addr, char **word)
{
	int		simple_q;
	int		double_q;
	int		char_count;
	char	*save_startword;

	simple_q = 0;
	double_q = 0;
	char_count = 0;
	save_startword = startword;
	while (*startword)
	{
		char_count += tree_named_env(&startword, word);
		if ((*startword == ' ' || *startword == '>' || *startword == '<'
			|| *startword == '|' || *startword == ';') && !(simple_q || double_q))
			break ;
		char_count += tree_named_backslash(&startword, word, simple_q, double_q);
		tree_named_quote(startword, word, &simple_q, &double_q);
		if (!(tree_named_last(&startword, word)))
			return (ERROR);
		char_count++;
		if (*(startword) == '\0' && (simple_q || double_q))
		{
			/*** WIP ***/
			// free à faire
			if (simple_q)
				ft_printf("squote > ");
			else
				ft_printf("dquote > ");
			char *test = edit_line();
			int startword_advencement = startword - save_startword;
			test = ft_strjoin("\n", test);
			startword = ft_strjoin(save_startword, test);
			*entry_addr = ft_strjoin(*entry_addr, test);
			save_startword = startword;
			startword += startword_advencement;
			continue;
		}
	}

	if (word && *word && is_special_token(*word) == SUCCESS)
		*word = add_char_to_word_free(*word, 3);
	return (char_count);
}
