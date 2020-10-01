/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 19:29:27 by thervieu          #+#    #+#             */
/*   Updated: 2020/10/01 16:45:08 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int		treat_entry(char *cmd)
{
	char	**cmd_treated;
	int		*sequence;
	int		cursor;

	cmd_treated = NULL;
	sequence = NULL;
	cursor = 0;
	if (!sanitize(cmd, &cmd_treated))
	{
		g_ms.last_cmd_rtn = 2;
		return (ERROR);
	}
	g_ms.has_pipe = 0;
	if (get_sequence(cmd_treated, &sequence) != SUCCESS)
		return (ERROR);
	reorder_sequence(&cmd_treated, &sequence);
	while (cmd_treated[cursor])
		g_ms.has_pipe += (sequence[cursor++] == 6) ? 1 : 0;
	g_ms.has_pipe += (g_ms.has_pipe) ? 1 : 0;
	(g_ms.has_pipe == 0) ?
		cmd_no_pipe(cmd_treated, sequence)
			: cmd_has_pipe(cmd_treated, sequence);
	free_double_char_tab(cmd_treated);
	free(sequence);
	return (SUCCESS);
}

int		precheck_norm(char *entry)
{
	if (*entry && *entry == ';')
	{
		if (*(entry + 1) && *(entry + 1) == ';')
			ft_printf(2,
				"minishell: syntax error near unexpected token « ;; »\n");
		else
			ft_printf(2,
				"minishell: syntax error near unexpected token « ; »\n");
		return (ERROR);
	}
	return (SUCCESS);
}

int		entry_splitter_precheck(
	char *entry
)
{
	int quote;

	quote = 0;
	if (precheck_norm(entry) == ERROR)
		return (ERROR);
	while (*entry)
	{
		if (*entry == '\'' && *(entry - 1) != '\\' && quote == 0)
			quote = 1;
		else if (*entry == '\'' && *(entry - 1) != '\\' && quote == 1)
			quote = 0;
		if (*entry == '"' && *(entry - 1) != '\\' && quote == 0)
			quote = 2;
		else if (*entry == '"' && *(entry - 1) != '\\' && quote == 2)
			quote = 0;
		if ((quote != 1 && quote != 2) && *entry && *entry == ';'
			&& *(entry + 1) == ';')
		{
			ft_printf(2,
				"minishell: syntax error near unexpected token « ;; »\n");
			return (ERROR);
		}
		entry++;
	}
	return (SUCCESS);
}

void	find_semicolon(char *new_start, char **f, int *s_quote, int *d_quote)
{
	char *cp;

	cp = new_start;
	while (*cp && cp != *f)
	{
		if (*cp == '\\')
		{
			(*(cp + 1) && (cp + 1) == *f) ? (*f = ft_strchr(*f + 1, ';')) : 0;
			cp += 1;
		}
		if (*cp == '\'' && *(cp - 1) != '\\'
			&& (*s_quote == 0 && *d_quote == 0))
			*s_quote = 1;
		else if (*cp == '\'' && *s_quote == 1)
			*s_quote = 0;
		if (*cp == '"' && *(cp - 1) != '\\' && (*d_quote == 0 && *s_quote == 0))
			*d_quote = 1;
		else if (*cp == '"' && *(cp - 1) != '\\' && *d_quote == 1)
			*d_quote = 0;
		if (cp && *(cp + 1) && (cp + 1) == *f && (*s_quote == 1
			|| *d_quote == 1))
			(**f && (**f + 1)) ? (*f = ft_strchr(*f + 1, ';')) : 0;
		cp++;
	}
	(!*f) ? (*f = (new_start + ft_secure_strlen(new_start))) : 0;
}

int		entry_splitter(
	char *entry,
	int s_quote,
	int d_quote,
	char *cmd
)
{
	char *find;
	char *new_start;

	find = NULL;
	new_start = entry;
	if (entry_splitter_precheck(new_start) == ERROR)
		return (g_ms.last_cmd_rtn = 2);
	while (*new_start)
	{
		s_quote = 0;
		d_quote = 0;
		find = ft_strchr(new_start, ';');
		(!find) ? (find = (new_start + ft_secure_strlen(new_start))) : 0;
		find_semicolon(new_start, &find, &s_quote, &d_quote);
		cmd = (find != '\0') ? ft_substr(entry,
			new_start - entry, (find - new_start) + 1)
			: ft_substr(entry, new_start - entry, find - new_start);
		if (treat_entry(cmd) == ERROR)
			return (g_ms.last_cmd_rtn = 2);
		(cmd) ? free(cmd) : 0;
		new_start = (find && *find && *(find + 1) != 0) ? (find + 1) : 0;
		if (new_start == 0)
			break ;
	}
	return (SUCCESS);
}
