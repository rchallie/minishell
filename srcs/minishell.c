/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/13 16:03:58 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static char	**get_cmd_arguments(void)
{
	char	**rtn;
	int		cursor;

	rtn = NULL;
	cursor = ms.seq_cursor;
	add_word_to_tab(ms.treated[cursor], &rtn);
	if (ms.sequence[cursor + 1] == 2)
	{
		cursor++;
		while (ms.sequence[cursor] == 2)
			add_word_to_tab(ms.treated[cursor++], &rtn);
	}
	return (rtn);
}

int			treat_command(void)
{
	/** &cd, &print_work_dir, &exit_minishell, &env, **/
	static int	(*cmd[7])(int argc, char **argv, char **envp) = {
		&echo_, &print_work_dir, &exit_minishell, &env, &cd, &export_, &unset};
	char **argv;

	argv = NULL;
	if (ms.sequence[ms.seq_cursor] == 0
		&& (ms.isexecret = is_exec()) == ERROR)
	{
		if ((ms.iscmdret = is_cmd(ms.treated[ms.seq_cursor])) != -1
			&& (ms.iscmdret >= 0 && ms.iscmdret <= 6))
		{
			argv = get_cmd_arguments();
			ms.last_cmd_rtn = cmd[ms.iscmdret](get_double_char_tab_len(argv),
				argv, envp);
			free_double_char_tab(argv);
		}
		else if (ms.sequence[ms.seq_cursor] == 0
			&& ms.iscmdret == -1 && ms.treated[ms.seq_cursor][0])
				return (ERROR);
	}
	return (SUCCESS);
}

static void		cmd_no_pipe(void)
{
	int			saved_stdout;
	int			saved_stdin;
	int			fdinput;
	int			fdoutput;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	fdoutput = has_redir_output(0, ms.seq_cursor + 1, STDOUT_FILENO);
	fdinput = has_redir_input(0, ms.seq_cursor + 1, STDIN_FILENO);
	dup2(fdinput, STDIN_FILENO);
	dup2(fdoutput, STDOUT_FILENO);
	while (ms.seq_cursor < ms.treated_len && ms.treated[ms.seq_cursor])
	{
		if (treat_command() == ERROR)
			error_command(ms.treated[ms.seq_cursor]);
		ms.seq_cursor++;
	}
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

int			treat_entry(void)
{
	int seq_ret;

	ms.treated_len = get_double_char_tab_len(ms.treated);
	if ((seq_ret = get_sequence(ms.treated, &ms.sequence)) != SUCCESS)
		return (ERROR);
/*
** REORDER DEVRA CHECKER LA VALIDITER DES FILES DE REDIR
*/
	reorder_sequence();
	while (ms.treated[ms.cursor])
		ms.has_pipe += (ms.sequence[ms.cursor++] == 6) ? 1 : 0;
	ms.has_pipe += (ms.has_pipe) ? 1 : 0;
	(ms.has_pipe == 0) ? cmd_no_pipe() : cmd_has_pipe(0, 0, 0);
	free_double_char_tab(ms.treated);
	free(ms.sequence);
	return (SUCCESS);
}

static void		sigint_catcher()
{
	write(1, "\n", 1);
	ms.last_cmd_rtn = 130;
}

// ctrl-\" a gérer?
int				main(int ac, char **av, char **env)
{
	int			ret;
	char		*pwd;
	char		*execute_path_env;
	int			cmd_ret;
	// DEAL WITH ./ms < file
	cmd_ret = 0;
	(void)ac;
	(void)av;
	ret = 1;
	put_beg();
	dup_double_char_tab(env, &envp);
	// envp = env;
	if (signal(SIGINT, sigint_catcher) == SIG_ERR)
		exit(ERROR_SIGINT);
	while (ret == SUCCESS)
	{
		ms = (t_minishell){.iscmdret = -1, .isexecret = -1,
			.last_cmd_rtn = cmd_ret};
		if (!get_pwd_short(&pwd))
			return (ERROR);
		ft_printf("\e[97m[\e[91mm\e[92mi\e[93mn\e[94mi\e[95ms\e[96mh\e[91me");
		ft_printf("\e[92ml\e[93ml\e[97m] \e[91m%s \e[97m: ", pwd);
		free(pwd);
		if (!get_pwd(&pwd))
			return (ERROR);
		execute_path_env = ft_strjoin("_=", pwd);
		add_var_to_env(execute_path_env);
		free(execute_path_env);
		ret = line_edition(&ms);
		if (!sanitize(ms.entry, &ms.treated))
		{
			free_double_char_tab(ms.treated);
			free(ms.sequence);
			cmd_ret = 2;
			continue;
		}
		treat_entry();
		cmd_ret = ms.last_cmd_rtn;
	}
	(void)cmd_ret;
	return (0);
}
