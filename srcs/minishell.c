/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/21 15:44:00 by excalibur        ###   ########.fr       */
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
	while (g_ms.treated[g_ms.cursor])
		g_ms.has_pipe += (g_ms.sequence[g_ms.cursor++] == 6) ? 1 : 0;
	g_ms.has_pipe += (g_ms.has_pipe) ? 1 : 0;
	(g_ms.has_pipe == 0) ? cmd_no_pipe() : cmd_has_pipe(0, 0, 0);
	free_double_char_tab(g_ms.treated);
	free(g_ms.sequence);
	return (SUCCESS);
}

int		print_prompt(void)
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
	char		*pwd;
	char		*execute_path_env;
	int			rtn;

	g_ms = (t_minishell){.iscmdret = -1, .isexecret = -1,
		.last_cmd_rtn = *cmd_ret};
	if (!(g_ms.entry = ft_strnew(sizeof(char) * 65535)))
		return (ERROR);
	if (print_prompt() == ERROR || !get_pwd(&pwd))
		return (ERROR);
	execute_path_env = ft_strjoin("_=", pwd);
	add_var_to_env(execute_path_env);
	free(execute_path_env);
	
	rtn = 0;
	if ((rtn += read(0, &g_ms.entry[rtn], 65534)) == -1)
		return (errno);
	if (rtn)
	{
		while (g_ms.entry[rtn - 1] != '\n')
			if ((rtn += read(0, &g_ms.entry[rtn], 65534)) == -1)
				exit(errno);
	}
	else
	{
		ft_printf(1, "exit\n");
		ft_strcpy(g_ms.entry, "exit");
	}
	
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
	int			ret;
	int			cmd_ret;

	(void)ac;
	(void)av;
	ret = 1;
	cmd_ret = 0;
	dup_double_char_tab(env, &g_envp);
	if (signal(SIGINT, sigint_catcher) == SIG_ERR)
		exit(ERROR_SIGINT);
	if (signal(SIGQUIT, sigquit_catcher) == SIG_ERR)
		exit(ERROR_SIGQUIT);
	while (ret == SUCCESS)
		if (minishell_loop(&cmd_ret) == ERROR)
			return (1);
	return (0);
}
