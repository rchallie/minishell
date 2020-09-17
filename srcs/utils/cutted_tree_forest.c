/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cutted_tree_forest.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/21 16:10:38 by excalibur         #+#    #+#             */
/*   Updated: 2020/08/12 23:24:47 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int tree_named_env(char **entry, char **word)
{
	int rtn;
	char *env_var_name;

	env_var_name = NULL;
	rtn = 0;
	// ft_printf(1, "Actual char (pre) = %c (%.12s)| %d\n", **entry, *entry, **entry);
	if (**entry == '$' && *(*entry + 1) && *(*entry + 1) != ' ')
	{
		(*entry)++;
		rtn++;
		while (**entry && **entry != '\'' && **entry != '\"' && **entry != '=' && **entry != '$'
			&& *(*entry - 1) != '?' && (ft_isalnum(**entry) || **entry == '?'
			|| **entry == '_'))
		{
			env_var_name = add_char_to_word_free(env_var_name, **entry);
			(*entry)++;
			rtn++;
			if (ft_isdigit(*(*entry - 1)) && *(*entry - 2) == '$')
				break ;
		}
		if (env_var_name && ft_secure_strlen(env_var_name) == 1
			&& env_var_name[0] == '?')								 //Last return
		{
			env_var_name = (g_ms.last_cmd_rtn != -1) ?
				ft_itoa(g_ms.last_cmd_rtn) : ft_strdup("0");
		}
		else if (env_var_name)										// Get en var normaly
			env_var_name = get_env_var_by_name(env_var_name);
		if (env_var_name && *env_var_name)
		{
			*word = ft_strjoin(*word, env_var_name);
			*word = add_char_to_word_free(*word, '\0');
		}
		if (*(*entry - 1) == '$' && (**entry == '=' || **entry == '\"'))
			*word = add_char_to_word_free(*word, '$');
	}
	else if (**entry == '$')
		*word = add_char_to_word_free(*word, '$');
	// ft_printf(1, "Actual char (end) = %c (%.12s)| %d\n", **entry, *entry, **entry);
	//ft_printf(1, "wordaaa = |%s|\n", *word);
	return (rtn);
}

static int		no_quotes(char **entry, char **word, int *simple_q,
	int *double_q)
{
	if (**entry == '\'')
	{
		*simple_q = 1;
		if (!(*word))
			*word = ft_strdup("");
	}
	else if (**entry == '\"')
	{
		*double_q = 1;
		if (!(*word))
			*word = ft_strdup("");
	}
	else if (**entry == '\\')
	{
		//ft_printf(1, "startword3bis = |%s|\n", *entry);
		(*entry)++;
		//ft_printf(1, "startword3bis = |%s|\n", *entry);
		*word = add_char_to_word_free(*word, **entry);
		//ft_printf(1, "word = |%s|\n", *word);
		return (1);
	}
	else if (**entry != '$')
		*word = add_char_to_word_free(*word, **entry);
	return (0);
}

int				if_quotes(char **entry, char **word, int *simple_q,
	int *double_q)
{
	if (*simple_q == 0 && *double_q == 0)
		return (no_quotes(entry, word, simple_q, double_q));
	else if (*simple_q == 1)
		if (**entry == '\'')
			*simple_q = 0;
		else
			*word = add_char_to_word_free(*word, **entry);
	else if (*double_q == 1)
	{
		if (**entry == '\\' && (*(*entry + 1) == '\"' || *(*entry + 1) == '$' || *(*entry + 1) == '\\'))
		{
			(*entry)++;
			*word = add_char_to_word_free(*word, **entry);
			return (1);
		}
		else if (**entry == '\"')
			*double_q = 0;
		else if (**entry != '$')
			*word = add_char_to_word_free(*word, **entry);
	}
	return (0);
}

static void		quote_error(char **startword, char **entry_addr,
char **save_startword, int simple_q)
{
	char		*test;
	int			startword_advencement;

	(simple_q == 1) ? ft_printf(STDOUT_FILENO, "squote > ")
		: ft_printf(STDOUT_FILENO, "dquote > ");
	test = edit_line();
	startword_advencement = *startword - *save_startword;
	test = ft_strjoin("\n", test);
	*startword = ft_strjoin(*save_startword, test);
	*entry_addr = ft_strjoin(*entry_addr, test);
	*save_startword = *startword;
	*startword += startword_advencement;
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
	if (ft_secure_strlen(startword) == 1 && *startword == '$')
	{
		*word = add_char_to_word_free(*word, *startword);
		return (1);
	}
	//ft_printf(1, "startword1 = |%s|\n", startword);
	while (*startword)
	{
		//ft_printf(1, "startword2 = |%s|\n", startword);
		if (*startword == '$' && simple_q == 0)
			char_count += tree_named_env(&startword, word);
		//ft_printf(1, "startword2 = |%s|\n", startword);
		if (((*startword == ' ' || *startword == '>' || *startword == '<'
			|| *startword == '|' || *startword == ';')
			&& !(simple_q || double_q)) && word)
			break ;
		//ft_printf(1, "startword3 = |%s|\n", startword);
		char_count += if_quotes(&startword, word, &simple_q, &double_q);
		//ft_printf(1, "startword4 = |%s|\n", startword);
		if (*startword && (*startword != '$' || (*startword == '$'
			&& (*(startword - 1) == '\\' || *(startword + 1) == ' '))))
		{
			startword++;
			char_count++;
		}
		//ft_printf(1, "startword5 = |%s|\n", startword);
		if (*startword == '\0' && (simple_q || double_q))
			quote_error(&startword, entry_addr, &save_startword, simple_q);
	}
	if (word && *word && (is_special_token(*word) == SUCCESS))
		*word = add_char_to_word_free(*word, 3);
	// //ft_printf(1, "Char count = %d\n", char_count);
	//ft_printf(1, "startword6 = |%s|\n", startword);
	//ft_printf(1, "word = |%s|\n", *word);
	return (char_count);
}
