/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/10/01 18:37:27 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int			print_prompt(void)
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

static void	exit_negative_read(void)
{
	int		*int_exit;
	char	**exit;

	exit = new_double_char_tab_init(1, "exit");
	exit[1] = NULL;
	get_sequence(exit, &int_exit);
	treat_command(exit, int_exit, 0);
	return ;
}

static int	minishell_loop_isatty(
	int rtn,
	char *save,
	char *buffer
)
{
	if (print_prompt() == ERROR)
		return (ERROR);
	if (!(buffer = ft_strnew(sizeof(char) * (2))))
		return (-1);
	while (42)
	{
		if ((rtn = read(0, buffer, 1)) == -1)
		{
			(buffer) ? free(buffer) : 0;
			(save) ? free(save) : 0;
			exit_negative_read();
		}
		save = g_ms.entry;
		if (buffer != NULL && *buffer == '\n')
			break ;
		if (buffer != NULL)
			g_ms.entry = ft_strjoin(save, buffer);
		(save && save != g_ms.entry) ? free(save) : 0;
		(ft_strlen(g_ms.entry) == 0 && rtn == 0) ? exit_negative_read() : 0;
		ft_bzero(buffer, sizeof(char) * (2));
	}
	(buffer) ? free(buffer) : 0;
	if (g_ms.entry != NULL)
		entry_splitter(g_ms.entry, 0, 0, NULL);
	return (SUCCESS);
}

/*
** Function: minishell_loop
** ------------
**		Init global structur of variables. Call splitter
**		to treate "entry" or the  entry  from  the  line
**		edition, according to isatty.
**
**		params:
**			"isatty" : if it's set to 0 the entry will be
**					   voided and the used will be set from
**					   line edition, otherwise if it's 1
**					   "entry" will be used.
**			"entry"	 : the entry to use (defined by main function).
**			"cmd_ret": the address were store the return of th
**					   command.
*/

static int	minishell_loop(int isatty, char *entry, int *cmd_ret)
{
	sigcatcher_init();
	g_ms = (t_minishell){.iscmdret = 0, .isexecret = -1,
		.last_cmd_rtn = *cmd_ret, .entry = NULL};
	add_var_to_env("_=./minishell");
	if (isatty == 0)
	{
		if (minishell_loop_isatty(0, NULL, NULL) == ERROR)
			return (ERROR);
	}
	else
		entry_splitter(entry, 0, 0, NULL);
	*cmd_ret = g_ms.last_cmd_rtn;
	return (SUCCESS);
}

/*
** Minishell :
** -----------
**		Minishell is a project from 42 School. Please
**		take a look :  https://www.42.fr/.  The  main
**		objectives is to recreate a  little  part  of
**		bash :
**			- Built in :
**				+ cd
**				+ echo
**				+ env
**				+ exit
**				+ export
**				+ pwd
**				+ unset
**			- Can execute other built in.
**			- Pipes
**			- Semicolon
**			- Input / output / "overwrite output" file
**			  in command line.
**
**		Main funtion check that the file  descritor  is
**		linked to a terminal. If it's not, it will just
**		execute commands line from the file descriptor,
**		otherwise  it  open  minishell  and  wait   for
**		commands.
**
**		params:
**			"ac"	: the argument. count.
**			"av"	: the arguments array.
**			"env"	: the environments variables.
*/

int			main(int ac, char **av, char **env)
{
	int			cmd_ret;
	char		*line;

	(void)ac;
	(void)av;
	line = NULL;
	if (beg_pwd(env) == ERROR)
		return (ERROR);
	if (isatty(0) && beg_shlvl())
	{
		while (42)
			if (minishell_loop(0, g_ms.entry, &cmd_ret) == ERROR)
				return (1);
	}
	else if (beg_shlvl())
		while (get_next_line(0, &line) > 0)
		{
			if (minishell_loop(1, line, &cmd_ret) == ERROR)
				return (1);
			cmd_ret = g_ms.last_cmd_rtn;
			(line) ? free(line) : 0;
			line = NULL;
		}
	(line) ? free(line) : 0;
	return (g_ms.last_cmd_rtn);
}
