/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cutted_tree_forest.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/21 16:10:38 by excalibur         #+#    #+#             */
/*   Updated: 2020/04/30 15:56:56 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int		tree_named_env(t_minishell *ms, char **entry, char **word)
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
			env_var_name = ft_itoa(ms->last_cmd_rtn);
		else
			env_var_name = get_env_var_by_name(env_var_name, envp);
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

int				get_word(t_minishell *ms, char *entry, char **word)
{
	int		simple_q;
	int		double_q;
	int		char_count;

	simple_q = 0;
	double_q = 0;
	char_count = 0;
	while (*entry)
	{
		char_count += tree_named_env(ms, &entry, word);
		if ((*entry == ' ' || *entry == '>' || *entry == '<'
			|| *entry == '|' || *entry == ';') && !(simple_q || double_q))
			break ;
		char_count += tree_named_backslash(&entry, word, simple_q, double_q);
		tree_named_quote(entry, word, &simple_q, &double_q);
		if (!(tree_named_last(&entry, word)))
			return (ERROR);
		char_count++;
	}
	if (simple_q || double_q)
		exit(1);
	if (word && *word && is_special_token(*word) == SUCCESS)
		*word = add_char_to_word_free(*word, 3);
	return (char_count);
}
