/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize_env.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/26 23:57:30 by excalibur         #+#    #+#             */
/*   Updated: 2020/09/27 18:54:57 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int		test_d_quote_prev(
	char **entry,
	char *dollar_pos
)
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

static int		san_get_env_var_name(
	char **entry,
	char **env_var_name,
	int *accolade,
	int *rtn
)
{
	if (**entry == '{')
	{
		(*entry)++;
		(*rtn)++;
		*accolade = 1;
	}
	while (**entry && **entry != '}' && **entry != '\'' && **entry != '\"'
		&& **entry != '=' && **entry != '$' && *(*entry - 1) != '?'
		&& (ft_isalnum(**entry) || **entry == '?' || **entry == '_'))
	{
		if ((*env_var_name = add_char_to_word_free(*env_var_name, **entry))
			== ERROR)
			return (-1);
		(*entry)++;
		(*rtn)++;
		if (ft_isdigit(*(*entry - 1)) && *(*entry - 2) == '$')
			break ;
	}
	return (SUCCESS);
}

static int		san_get_env_var(
	char **env_var_name,
	char **entry,
	int accolade,
	int *rtn
)
{
	if (*env_var_name && ft_secure_strlen(*env_var_name) == 1
		&& *env_var_name[0] == '?')
	{
		*env_var_name = (g_ms.last_cmd_rtn != -1) ?
			ft_itoa(g_ms.last_cmd_rtn) : ft_strdup("0");
	}
	else if (*env_var_name
		&& ((accolade == 1 && **entry == '}') || (accolade == 0)))
	{
		*env_var_name = get_env_var_by_name(*env_var_name);
		if (accolade == 1)
		{
			(*entry)++;
			(*rtn)++;
		}
	}
	else if (accolade == 1 && **entry != '}')
	{
		ft_printf(2,
			"minishell: unexpected EOF while looking for matching `}'\n");
		ft_printf(2, "minishell: syntax error: unexpected end of file\n");
		return (-1);
	}
	return (SUCCESS);
}

static int		san_env_var_protocol(
	char **entry,
	char **word,
	int *rtn
)
{
	int		accolade;
	char	*env_var_name;
	char	*save_env;

	accolade = 0;
	env_var_name = NULL;
	if (san_get_env_var_name(entry, &env_var_name, &accolade, rtn) == -1)
		return (-1);
	save_env = env_var_name;
	if (san_get_env_var(&env_var_name, entry, accolade, rtn) == -1)
	{
		(save_env) ? free(save_env) : 0;
		return (-1);
	}
	(save_env) ? free(save_env) : 0;
	if (env_var_name && *env_var_name)
		*word = ft_strjoin(*word, env_var_name);
	(env_var_name) ? free(env_var_name) : 0;
	if (*(*entry - 1) == '$' && (**entry == '=' || **entry == '\"'))
		*word = add_char_to_word_free(*word, '$');
	return (SUCCESS);
}

int				sanitize_env_var(char **entry, char **word, char **entry_addr)
{
	int		rtn;

	rtn = 0;
	if (**entry == '$' && *(*entry + 1) && *(*entry + 1) != ' '
		&& *(*entry + 1) != '\\' && *(*entry + 1) != '"')
	{
		(*entry)++;
		rtn++;
		if (san_env_var_protocol(entry, word, &rtn) == -1)
			return (-1);
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
