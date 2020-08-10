/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:38:48 by rchallie          #+#    #+#             */
/*   Updated: 2020/08/09 18:09:28 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static char		**get_cmd_arguments(char **cmd, int *seq)
{
	char	**rtn;
	int		cursor;

	rtn = NULL;
	cursor = 0;
	add_word_to_tab(cmd[cursor], &rtn);
	if (seq[cursor + 1] == 2)
	{
		cursor++;
		while (seq[cursor] == 2)
			add_word_to_tab(cmd[cursor++], &rtn);
	}
	return (rtn);
}

int				treat_command(char **cmd, int *seq)
{
	static int	(*builtin[7])(int argc, char **argv, char **envp) = {
		&echo_, &print_work_dir, &exit_minishell, &env, &cd, &export_, &unset};
	char		**argv;
	int			cursor;

	argv = NULL;
	cursor = 0;
	if (seq[cursor] == 0
		&& (g_ms.isexecret = is_exec(cmd, seq)) == ERROR)
	{
		if ((g_ms.iscmdret = is_cmd(cmd[cursor])) != -1
			&& (g_ms.iscmdret >= 0 && g_ms.iscmdret <= 6))
		{
			argv = get_cmd_arguments(cmd, seq);
			g_ms.last_cmd_rtn =
				builtin[g_ms.iscmdret](get_double_char_tab_len(argv),
				argv, g_envp);
			free_double_char_tab(argv);
		}
		else if (seq[cursor] == 0
			&& g_ms.iscmdret == -1 && cmd[cursor][0])
			return (ERROR);
	}
	return (SUCCESS);
}

void			cmd_no_pipe(char **cmd, int *seq)
{
	int			saved_stdout;
	int			saved_stdin;
	int			fdinput;
	int			fdoutput;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if ((fdoutput = has_redir_output(0,
		1, STDOUT_FILENO, cmd, seq)) == -1
	|| (fdinput = has_redir_input(0,
		1, STDIN_FILENO, cmd, seq)) == -1)
		return ;
	dup2(fdinput, STDIN_FILENO);
	dup2(fdoutput, STDOUT_FILENO);
	if (treat_command(cmd, seq) == ERROR)
		error_command(cmd[0]);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

int				has_redir_output(int redir_type,
	int cursor, int fd, char **cmd, int *seq)
{
	int			o;
	int			s;

	s = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if (!seq[cursor] || seq[cursor] == 6
		|| seq[cursor] == 7 || seq[cursor] == 9)
		return (fd);
	else if (seq[cursor] == 2)
		return (has_redir_output(redir_type, cursor + 1, fd, cmd, seq));
	else if (seq[cursor] == 4 || seq[cursor] == 3)
		return (has_redir_output(seq[cursor], cursor + 1, fd, cmd, seq));
	else if (seq[cursor] == 8 && (redir_type == 4 || redir_type == 3))
	{
		(fd >= 3) ? close(fd) : 0;
		o = (redir_type == 3) ? O_CREAT | O_RDWR | O_TRUNC
			: O_CREAT | O_RDWR | O_APPEND;
		if ((fd = open(cmd[cursor], o, s)) == -1)
			return (error_file(cmd[cursor], errno));
		redir_type = 0;
	}
	return (has_redir_output(redir_type, cursor + 1, fd, cmd, seq));
}

int				has_redir_input(int redir_type,
	int cursor, int fd, char **cmd, int *seq)
{
	if (!seq[cursor] || seq[cursor] == 6
		|| seq[cursor] == 7 || seq[cursor] == 9)
		return (fd);
	else if (seq[cursor] == 2)
		return (has_redir_input(redir_type, cursor + 1, fd, cmd, seq));
	else if (seq[cursor] == 5)
		return (has_redir_input(seq[cursor], cursor + 1, fd, cmd, seq));
	else if (seq[cursor] == 8 && redir_type == 5)
	{
		(fd >= 3) ? close(fd) : 0;
		if ((fd = open(cmd[cursor], O_RDONLY)) == -1)
			return (error_file(cmd[cursor], errno));
		redir_type = 0;
	}
	return (has_redir_input(redir_type, cursor + 1, fd, cmd, seq));
}
