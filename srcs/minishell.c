/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/29 18:50:39 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int				treat_entry(char *cmd)
{
	char **cmd_treated;
	int *sequence;
	int cursor;

	cmd_treated = NULL;
	sequence = NULL;
	cursor = 0;
	ft_printf(1, "a\n");
	if (!sanitize(cmd, &cmd_treated))
	{
		g_ms.last_cmd_rtn = 2;
		return (ERROR);
	}
	ft_printf(1, "b\n");

	int e = 0;
	while (cmd_treated[e])
	{
		ft_printf(1, "CMD = |%s|\n", cmd_treated[e]);
		e++;
	}

	g_ms.has_pipe = 0;
	
	if (get_sequence(cmd_treated, &sequence) != SUCCESS)
		return (ERROR);
	e = 0;
	while (cmd_treated[e])
	{
		ft_printf(1, "SEQ = |%d|\n", sequence[e]);
		e++;
	}
	ft_printf(1, "c\n");
	reorder_sequence(&cmd_treated, &sequence);
	ft_printf(1, "d\n");
	
	while (cmd_treated[cursor])
		g_ms.has_pipe += (sequence[cursor++] == 6) ? 1 : 0;
	ft_printf(1, "E\n");
	g_ms.has_pipe += (g_ms.has_pipe) ? 1 : 0;
	(g_ms.has_pipe == 0) ?
		cmd_no_pipe(cmd_treated, sequence) : cmd_has_pipe(0, 0, 0, cmd_treated, sequence);
	ft_printf(1, "F\n");
	
	free_double_char_tab(cmd_treated);
	free(sequence);

	/*
	int seq_ret;

	g_ms.treated_len = get_double_char_tab_len(g_ms.treated);
	if ((seq_ret = get_sequence(g_ms.treated, &g_ms.sequence)) != SUCCESS)
		return (ERROR);
	reorder_sequence();

	int cursor = 0;
		char **cmd = NULL;
		int *seq;
		g_ms.has_pipe = 0;
		get_sequence(cmd, &seq);

		int cmd_cursor = 0;
		while (cmd[cmd_cursor])
			g_ms.has_pipe += (g_ms.sequence[cmd_cursor++] == 6) ? 1 : 0;
		g_ms.has_pipe += (g_ms.has_pipe) ? 1 : 0;
		(g_ms.has_pipe == 0) ? cmd_no_pipe(cmd, seq) : cmd_has_pipe(0, 0, 0, cmd, seq);
		free(seq);
		free_double_char_tab(cmd);
		//ft_printf(1, "Cursor = %d\n", cursor);

	// while (g_ms.treated[g_ms.cursor])
	// 	g_ms.has_pipe += (g_ms.sequence[g_ms.cursor++] == 6) ? 1 : 0;
	// g_ms.has_pipe += (g_ms.has_pipe) ? 1 : 0;
	// (g_ms.has_pipe == 0) ? cmd_no_pipe() : cmd_has_pipe(0, 0, 0);
	free_double_char_tab(g_ms.treated);
	free(g_ms.sequence);*/
	return (SUCCESS);
}

static int		print_prompt(void)
{
	char *pwd;

	if (!get_pwd_short(&pwd))
		return (ERROR);
	ft_printf(STDOUT_FILENO,
		"\e[97m[\e[91mm\e[92mi\e[93mn\e[94mi\e[95ms\e[96mh\e[91me");
	ft_printf(STDOUT_FILENO,
		"\e[92ml\e[93ml\e[97m] \e[91m%s \e[97m: ", pwd);
	free(pwd);
	return (SUCCESS);
}

static int		minishell_loop(int *cmd_ret)
{
	g_ms = (t_minishell){.iscmdret = 0, .isexecret = -1,
		.last_cmd_rtn = *cmd_ret};
	if (print_prompt() == ERROR)
		return (ERROR);
	add_var_to_env("_=./minishell");
	line_edition();
	// int	quotes;
	// char	*cmd;
	// quotes = 0;
	// while (g_ms.entry)
	// {
	// 	while (g_ms.entry)
	// 	{
	// 		if (g_ms.entry == ';' && quotes == 0)
	// 			break ;
	// 		if_quotes()
	// 		g_ms.entry++;
	// 	}
	// 	if (g_ms.entry)
	// 		g_ms.entry++;
	// }

	char *cmd;
	char *find = NULL;
	char *new_start = g_ms.entry;
	while (*new_start)
	{
		ft_printf(1, "New_start = |%s|\n", new_start);
		find = ft_strchr(new_start, ';');
		ft_printf(1, "Pointer new_start = %p | next pointer find = %p\n", new_start, (new_start + ft_secure_strlen(new_start)) );
		if (!find)
			find = (new_start + ft_secure_strlen(new_start));
		ft_printf(1,"FInd -1 = %d\n", *find);
		ft_printf(1, "1\n");
		char *cp = new_start;
		int s_quote = 0;
		int d_quote = 0;
		ft_printf(1, "2\n");
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
		ft_printf(1, "3\n");
		if (s_quote == 1 || d_quote == 1)
			continue;
		ft_printf(1, "4\n");
		cmd = ft_substr(g_ms.entry, new_start - g_ms.entry, find - new_start);
		ft_printf(1, "5\n");
		ft_printf(1, "CMD = |%s|\n", cmd);
		treat_entry(cmd);
		free(cmd);
		ft_printf(1, "e\n");
		new_start = find;

		ft_printf(1, "f\n");
	}

	exit(1);
		
	*cmd_ret = g_ms.last_cmd_rtn;
	return (SUCCESS);
}

int				main(int ac, char **av, char **env)
{
	int			cmd_ret;

	(void)ac;
	(void)av;
	cmd_ret = 0;
	dup_double_char_tab(env, &g_envp);
	if (signal(SIGINT, sigint_catcher) == SIG_ERR)
		exit(ERROR_SIGINT);
	if (signal(SIGQUIT, sigquit_catcher) == SIG_ERR)
		exit(ERROR_SIGQUIT);
	(void)minishell_loop;
	if (isatty(0))
	{
		put_beg();
		while (42)
			if (minishell_loop(&cmd_ret) == ERROR)
				return (1);
	}
	int rtn = 0;
	char *line;
	while ((rtn = get_next_line(0, &line)) > 0)
	{
		g_ms = (t_minishell){.iscmdret = -1, .isexecret = -1,
		.last_cmd_rtn = cmd_ret, .entry = line};
		add_var_to_env("_=./minishell");
		
		/* NOUVEAU SPLITER */
		// {
		// 	treat_entry();
		// }
		// if (!sanitize(g_ms.entry, &g_ms.treated))
		// {
		// 	free_double_char_tab(g_ms.treated);
		// 	free(g_ms.sequence);
		// 	cmd_ret = 2;
		// 	return (SUCCESS);
		// }
		cmd_ret = g_ms.last_cmd_rtn;
	}

	return (0);
}
