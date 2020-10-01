/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   splitter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 19:29:27 by thervieu          #+#    #+#             */
/*   Updated: 2020/10/01 12:44:43 by excalibur        ###   ########.fr       */
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

int		entry_splitter_precheck(
	char *entry
)
{
	int s_quote;
	int d_quote;

	s_quote = 0;
	d_quote = 0;
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
	while (*entry)
	{
		if (*entry == '\'' && *(entry - 1) != '\\' && s_quote == 0)
			s_quote = 1;
		else if (*entry == '\'' && *(entry - 1) != '\\' && s_quote == 1)
			s_quote = 0;
		if (*entry == '"' && *(entry - 1) != '\\' && d_quote == 0)
			d_quote = 1;
		else if (*entry == '"' && *(entry - 1) != '\\' && d_quote == 1)
			d_quote = 0;
		if ((s_quote != 1 && d_quote != 1) && *entry && *entry == ';'
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

void	find_semicolon(char *new_start, char **find, int *s_quote, int *d_quote)
{
	char *cp;

	cp = new_start;
	while (*cp && cp != *find)
	{
		if (*cp == '\\')
		{
			if (*(cp + 1) && (cp + 1) == *find)
				*find = ft_strchr(*find + 1, ';');
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
		if (cp && *(cp + 1) && (cp + 1) == *find && (*s_quote == 1
			|| *d_quote == 1))
			if (**find && (**find + 1))
				*find = ft_strchr(*find + 1, ';');
		cp++;
	}
	(!*find) ? (*find = (new_start + ft_secure_strlen(new_start))) : 0;
}

int		entry_splitter(
	char *entry,
	int s_quote,
	int d_quote
)
{
	char *cmd;
	char *find;
	char *new_start;

	cmd = NULL;
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
