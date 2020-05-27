/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/26 17:34:52 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int				treat_entry(void)
{
	int seq_ret;

	g_ms.treated_len = get_double_char_tab_len(g_ms.treated);
	if ((seq_ret = get_sequence(g_ms.treated, &g_ms.sequence)) != SUCCESS)
		return (ERROR);
	reorder_sequence();

	int cursor = 0;
	while(g_ms.treated[cursor])
	{
		char **cmd = NULL;
		int *seq;
		g_ms.has_pipe = 0;
		while (g_ms.treated[cursor] && g_ms.sequence[cursor] != 7)
		{
			if (!add_word_to_tab(g_ms.treated[cursor], &cmd))
				return (ERROR);
			cursor++;
		}
		get_sequence(cmd, &seq);
		int n_cursor = 0;
		while (cmd[n_cursor])
		{
			ft_printf(1, "Cmd = %s | seq = %d\n", cmd[n_cursor], seq[n_cursor]);
			n_cursor++;
		}

		int cmd_cursor = 0;
		while (cmd[cmd_cursor])
			g_ms.has_pipe += (g_ms.sequence[cmd_cursor++] == 6) ? 1 : 0;
		g_ms.has_pipe += (g_ms.has_pipe) ? 1 : 0;
		(g_ms.has_pipe == 0) ? cmd_no_pipe(cmd, seq) : cmd_has_pipe(0, 0, 0, cmd, seq);
		free(seq);
		free_double_char_tab(cmd);
		if (g_ms.treated[cursor])
			cursor++;
		ft_printf(1, "Cursor = %d\n", cursor);
	}

	// while (g_ms.treated[g_ms.cursor])
	// 	g_ms.has_pipe += (g_ms.sequence[g_ms.cursor++] == 6) ? 1 : 0;
	// g_ms.has_pipe += (g_ms.has_pipe) ? 1 : 0;
	// (g_ms.has_pipe == 0) ? cmd_no_pipe() : cmd_has_pipe(0, 0, 0);
	free_double_char_tab(g_ms.treated);
	free(g_ms.sequence);
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
	if (!sanitize(g_ms.entry, &g_ms.treated))
	{
		free_double_char_tab(g_ms.treated);
		free(g_ms.sequence);
		*cmd_ret = 2;
		return (SUCCESS);
	}
	treat_entry();
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
		if (!sanitize(g_ms.entry, &g_ms.treated))
		{
			free_double_char_tab(g_ms.treated);
			free(g_ms.sequence);
			cmd_ret = 2;
			return (SUCCESS);
		}
		treat_entry();
		cmd_ret = g_ms.last_cmd_rtn;
	}

	return (0);
}
