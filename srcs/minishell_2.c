/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:38:48 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/27 23:47:29 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static char		**get_cmd_arguments(char **cmd, int *seq)
{
	char	**rtn;
	int		cursor;

	rtn = NULL;
	cursor = 0;
	while (cmd[cursor] && seq[cursor] != 0)
		cursor++;
	add_word_to_tab(cmd[cursor], &rtn);
	if (cmd[cursor] && seq[cursor + 1] == 2)
	{
		cursor++;
		while (seq[cursor] == 2)
			add_word_to_tab(cmd[cursor++], &rtn);
	}
	return (rtn);
}

int				treat_command(char **cmd, int *seq, int cursor)
{
	static int	(*builtin[7])(int argc, char **argv, char **envp) = {
		&echo_, &print_work_dir, &exit_minishell, &env, &cd, &export_, &unset};
	char		**argv;

	argv = NULL;
	while (cmd[cursor] && seq[cursor] != 0)
		cursor++;
	if (cmd[cursor] && seq[cursor] == 0
		&& (g_ms.isexecret = is_exec(cmd, seq)) == ERROR)
	{
		if ((g_ms.iscmdret = is_cmd(cmd[cursor])) != -1
			&& (g_ms.iscmdret >= 0 && g_ms.iscmdret <= 6))
		{
			argv = get_cmd_arguments(cmd, seq);
			g_ms.last_cmd_rtn = builtin[g_ms.iscmdret](
				get_double_char_tab_len(argv), argv, g_envp);
			free_double_char_tab(argv);
		}
		else if (seq[cursor] == 0 && g_ms.iscmdret == -1 && cmd[cursor][0])
		{
			g_ms.last_cmd_rtn = 127;
			return (ERROR);
		}
	}
	return (SUCCESS);
}

static int		handle_file(char *cmd, int *fdin, int *fdout, int *redir_type)
{
	int flags;
	int mask;

	flags = 0;
	mask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if (*redir_type == 4 || *redir_type == 3)
	{
		(*fdout >= 3) ? close(*fdout) : 0;
		flags = (*redir_type == 3) ? O_CREAT | O_RDWR | O_TRUNC
			: O_CREAT | O_RDWR | O_APPEND;
		if ((*fdout = open(cmd, flags, mask)) == -1)
			return (-1);
	}
	else if (*redir_type == 5)
	{
		(*fdin >= 3) ? close(*fdin) : 0;
		if ((*fdin = open(cmd, O_RDONLY)) == -1)
			return (-1);
	}
	*redir_type = 0;
	return (1);
}

int				has_redir(char **cmd, int *seq, int *fdin, int *fdout)
{
	int cursor;
	int redir_type;
	int flags;
	int mask;

	cursor = -1;
	redir_type = 0;
	flags = 0;
	mask = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	while (++cursor != -1)
	{
		if (cursor != 0 && (!seq[cursor] || seq[cursor] == 6
			|| seq[cursor] == 7 || seq[cursor] == 9))
			return (SUCCESS);
		else if (seq[cursor] == 2 || seq[cursor] == 4
			|| seq[cursor] == 3 || seq[cursor] == 5)
		{
			(seq[cursor] != 2) ? (redir_type = seq[cursor]) : 0;
			continue;
		}
		else if (seq[cursor] == 8 && (redir_type == 4 || redir_type == 3
			|| redir_type == 5))
			if (handle_file(cmd[cursor], fdin, fdout, &redir_type) == -1)
				return (error_file(cmd[cursor], errno));

	}
	return (ERROR);
}

void			cmd_no_pipe(char **cmd, int *seq)
{
	int			saved_stdout;
	int			saved_stdin;
	int			fdinput;
	int			fdoutput;

	saved_stdin = STDIN_FILENO;
	saved_stdout = STDOUT_FILENO;
	fdinput = STDIN_FILENO;
	fdoutput = STDOUT_FILENO;
	if (has_redir(cmd, seq, &fdinput, &fdoutput) != SUCCESS)
		return ;
	(fdinput != STDIN_FILENO) ? saved_stdin = dup(STDIN_FILENO) : 0;
	(fdoutput != STDOUT_FILENO) ? saved_stdout = dup(STDOUT_FILENO) : 0;
	dup2(fdinput, STDIN_FILENO);
	dup2(fdoutput, STDOUT_FILENO);
	(treat_command(cmd, seq, 0) == ERROR) ? error_command(cmd[0]) : 0;
	(fdinput != STDIN_FILENO) ? close(fdinput) : 0;
	(fdinput != STDIN_FILENO) ? dup2(saved_stdin, STDIN_FILENO) : 0;
	(fdinput != STDIN_FILENO) ? close(saved_stdin) : 0;
	(fdoutput != STDOUT_FILENO) ? close(fdoutput) : 0;
	(fdoutput != STDOUT_FILENO) ? dup2(saved_stdout, STDOUT_FILENO) : 0;
	(fdoutput != STDOUT_FILENO) ? close(saved_stdout) : 0;
}

