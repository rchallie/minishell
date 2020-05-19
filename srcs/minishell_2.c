/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/10 18:38:48 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/19 19:03:39 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static char		**get_cmd_arguments(void)
{
	char	**rtn;
	int		cursor;

	rtn = NULL;
	cursor = g_ms.seq_cursor;
	add_word_to_tab(g_ms.treated[cursor], &rtn);
	if (g_ms.sequence[cursor + 1] == 2)
	{
		cursor++;
		while (g_ms.sequence[cursor] == 2)
			add_word_to_tab(g_ms.treated[cursor++], &rtn);
	}
	return (rtn);
}

int				treat_command(void)
{
	static int	(*cmd[7])(int argc, char **argv, char **envp) = {
		&echo_, &print_work_dir, &exit_minishell, &env, &cd, &export_, &unset};
	char		**argv;

	argv = NULL;
	if (g_ms.sequence[g_ms.seq_cursor] == 0
		&& (g_ms.isexecret = is_exec()) == ERROR)
	{
		if ((g_ms.iscmdret = is_cmd(g_ms.treated[g_ms.seq_cursor])) != -1
			&& (g_ms.iscmdret >= 0 && g_ms.iscmdret <= 6))
		{
			argv = get_cmd_arguments();
			g_ms.last_cmd_rtn =
				cmd[g_ms.iscmdret](get_double_char_tab_len(argv), argv, g_envp);
			free_double_char_tab(argv);
		}
		else if (g_ms.sequence[g_ms.seq_cursor] == 0
			&& g_ms.iscmdret == -1 && g_ms.treated[g_ms.seq_cursor][0])
			return (ERROR);
	}
	return (SUCCESS);
}

void			cmd_no_pipe(void)
{
	int			saved_stdout;
	int			saved_stdin;
	int			fdinput;
	int			fdoutput;

	saved_stdout = dup(STDOUT_FILENO);
	saved_stdin = dup(STDIN_FILENO);
	if ((fdoutput = has_redir_output(0,
		g_ms.seq_cursor + 1, STDOUT_FILENO)) == -1
	|| (fdinput = has_redir_input(0,
		g_ms.seq_cursor + 1, STDIN_FILENO)) == -1)
		return ;
	dup2(fdinput, STDIN_FILENO);
	dup2(fdoutput, STDOUT_FILENO);
	while (g_ms.seq_cursor < g_ms.treated_len && g_ms.treated[g_ms.seq_cursor])
	{
		if (treat_command() == ERROR)
			error_command(g_ms.treated[g_ms.seq_cursor]);
		g_ms.seq_cursor++;
	}
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdout);
	dup2(saved_stdin, STDIN_FILENO);
	close(saved_stdin);
}

int				has_redir_output(int redir_type,
	int cursor, int fd)
{
	int			o;
	int			s;

	s = S_IRWXG | S_IRWXU | S_IRWXO;
	if (!g_ms.sequence[cursor] || g_ms.sequence[cursor] == 6
		|| g_ms.sequence[cursor] == 7 || g_ms.sequence[cursor] == 9)
		return (fd);
	else if (g_ms.sequence[cursor] == 2)
		return (has_redir_output(redir_type, cursor + 1, fd));
	else if (g_ms.sequence[cursor] == 4 || g_ms.sequence[cursor] == 3)
		return (has_redir_output(g_ms.sequence[cursor], cursor + 1, fd));
	else if (g_ms.sequence[cursor] == 8 && (redir_type == 4 || redir_type == 3))
	{
		(fd >= 3) ? close(fd) : 0;
		o = (redir_type == 3) ? O_CREAT | O_RDWR | O_TRUNC
			: O_CREAT | O_RDWR | O_APPEND;
		if ((fd = open(g_ms.treated[cursor], o, s)) == -1)
			return (error_file(g_ms.treated[cursor], errno));
		redir_type = 0;
	}
	return (has_redir_output(redir_type, cursor + 1, fd));
}

int				has_redir_input(int redir_type,
	int cursor, int fd)
{
	if (!g_ms.sequence[cursor] || g_ms.sequence[cursor] == 6
		|| g_ms.sequence[cursor] == 7 || g_ms.sequence[cursor] == 9)
		return (fd);
	else if (g_ms.sequence[cursor] == 2)
		return (has_redir_input(redir_type, cursor + 1, fd));
	else if (g_ms.sequence[cursor] == 5)
		return (has_redir_input(g_ms.sequence[cursor], cursor + 1, fd));
	else if (g_ms.sequence[cursor] == 8 && redir_type == 5)
	{
		(fd >= 3) ? close(fd) : 0;
		if ((fd = open(g_ms.treated[cursor], O_RDONLY)) == -1)
			return (error_file(g_ms.treated[cursor], errno));
		redir_type = 0;
	}
	return (has_redir_input(redir_type, cursor + 1, fd));
}
