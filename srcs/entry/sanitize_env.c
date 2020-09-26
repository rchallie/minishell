/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 23:57:30 by excalibur         #+#    #+#             */
/*   Updated: 2020/09/27 00:02:32 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int		test_d_quote_prev(char **entry, char *dollar_pos)
{
	int d_quote;

	d_quote = 0;
	while (**entry && *entry != dollar_pos)
	{
		d_quote = (d_quote == 0) ? 1 : 0;
		(*entry)++;
	}
	return (d_quote);
}

int				sanitize_env_var(char **entry, char **word, char **entry_addr)
{
	int		rtn;
	char	*env_var_name;
	int		accolade;
	char	*save_env;

	accolade = 0;
	env_var_name = NULL;
	rtn = 0;
	if (**entry == '$' && *(*entry + 1) && *(*entry + 1) != ' '
		&& *(*entry + 1) != '\\' && *(*entry + 1) != '"')
	{
		(*entry)++;
		rtn++;
		if (**entry == '{')
		{
			(*entry)++;
			rtn++;
			accolade = 1;
		}
		while (**entry && **entry != '}' && **entry != '\'' && **entry != '\"'
			&& **entry != '=' && **entry != '$' && *(*entry - 1) != '?'
			&& (ft_isalnum(**entry) || **entry == '?' || **entry == '_'))
		{
			env_var_name = add_char_to_word_free(env_var_name, **entry);
			(*entry)++;
			rtn++;
			if (ft_isdigit(*(*entry - 1)) && *(*entry - 2) == '$')
				break ;
		}
		save_env = env_var_name;
		if (env_var_name && ft_secure_strlen(env_var_name) == 1
			&& env_var_name[0] == '?')
		{
			env_var_name = (g_ms.last_cmd_rtn != -1) ?
				ft_itoa(g_ms.last_cmd_rtn) : ft_strdup("0");
		}
		else if (env_var_name
			&& ((accolade == 1 && **entry == '}') || (accolade == 0)))
		{
			env_var_name = get_env_var_by_name(env_var_name);
			if (accolade == 1)
			{
				(*entry)++;
				rtn++;
			}
		}
		else if (accolade == 1 && **entry != '}')
		{
			ft_printf(2,
				"minishell: unexpected EOF while looking for matching `}'\n");
			ft_printf(2, "minishell: syntax error: unexpected end of file\n");
			(save_env) ? free(save_env) : 0;
			return (-1);
		}
		(save_env) ? free(save_env) : 0;
		if (env_var_name && *env_var_name)
			*word = ft_strjoin(*word, env_var_name);
		(env_var_name) ? free(env_var_name) : 0;
		if (*(*entry - 1) == '$' && (**entry == '=' || **entry == '\"'))
			*word = add_char_to_word_free(*word, '$');
	}
	else if (**entry == '$')
	{
		if (!(test_d_quote_prev(entry_addr, *entry) == 1
			&& *(*entry + 1) == '"'))
			*word = add_char_to_word_free(*word, '$');
		(*entry)++;
	}
	return (rtn);
}