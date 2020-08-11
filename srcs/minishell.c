/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/08/11 11:46:57 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int				treat_entry(char *cmd)
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

static void		entry_splitter(
	char *entry
)
{
	char *cmd;
	char *find = NULL;
	char *new_start = entry;
	while (*new_start)
	{
		find = ft_strchr(new_start, ';');
		if (!find)
			find = (new_start + ft_secure_strlen(new_start));
		char *cp = new_start;
		int s_quote = 0;
		int d_quote = 0;
		while (*cp && *cp != *find)
		{
			if (*cp == '\'' && *(cp - 1) != '\\' && s_quote == 0)
				s_quote = 1;
			else if (*cp == '\'' && *(cp - 1) != '\\' && s_quote == 1)
				s_quote = 0;
			if (*cp == '"' && *(cp - 1) != '\\' && d_quote == 0)
				d_quote = 1;
			else if (*cp == '"' && *(cp - 1) != '\\' && d_quote == 1)
				d_quote = 0;
			cp++;
		}
		if (s_quote == 1 || d_quote == 1)
			continue;
		cmd = ft_substr(entry, new_start - entry, find - new_start);
		treat_entry(cmd);
		if (find && *find && *(find + 1) != 0)
			new_start = find + 1;
		else
			new_start = 0;
		if (new_start == 0)
			break ;
	}
}

static int		minishell_loop(char *entry, int *cmd_ret, int isatty)
{
	g_ms = (t_minishell){.iscmdret = 0, .isexecret = -1,
		.last_cmd_rtn = *cmd_ret};
	add_var_to_env("_=./minishell");
	if (isatty == 0)
	{
		(void)entry;
		if (print_prompt() == ERROR)
			return (ERROR);
		line_edition();
		entry_splitter(g_ms.entry);
	}
	else
		entry_splitter(entry);
	*cmd_ret = g_ms.last_cmd_rtn;
	return (SUCCESS);
}

int				main(int ac, char **av, char **env)
{
	int			cmd_ret;
	int			rtn;
	char		*line;

	(void)ac;
	(void)av;
	cmd_ret = 0;
	rtn = 0;
	dup_double_char_tab(env, &g_envp);
	sigcatcher_init();
	if (isatty(0))
	{
		put_beg();
		while (42)
			if (minishell_loop(g_ms.entry, &cmd_ret, 0) == ERROR)
				return (1);
	}
	else
		while ((rtn = get_next_line(0, &line)) > 0)
		{
			if (minishell_loop(line, &cmd_ret, 1) == ERROR)
				return (1);
			cmd_ret = g_ms.last_cmd_rtn;
		}
	return (0);
}
