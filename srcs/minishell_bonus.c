/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/10/01 16:21:05 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int				print_prompt(void)
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

static int		minishell_loop(int isatty, char *entry, int *cmd_ret)
{
	*cmd_ret = 0;
	g_ms = (t_minishell){.iscmdret = 0, .isexecret = -1,
		.last_cmd_rtn = *cmd_ret};
	add_var_to_env("_=./minishell");
	if (isatty == 0)
	{
		(void)entry;
		if (print_prompt() == ERROR)
			return (ERROR);
		line_edition();
		entry_splitter(g_ms.entry, 0, 0, NULL);
	}
	else
		entry_splitter(entry, 0, 0, NULL);
	*cmd_ret = g_ms.last_cmd_rtn;
	return (SUCCESS);
}

static int		beg_shlvl(void)
{
	int		i;
	int		shlvl;
	char	*shlvl_final;
	char	*shlvl_string;

	i = 0;
	shlvl = 0;
	shlvl_string = get_env_var_by_name("SHLVL");
	(shlvl_string[0] == '-' || shlvl_string[0] == '+') ? ++i : 0;
	while (shlvl_string[i] <= '9' && shlvl_string[i] >= '0')
		i++;
	if (shlvl_string[i] != '\0')
		shlvl = 1;
	else if (shlvl_string[0] == '-')
		shlvl = 0;
	else
		shlvl = ft_atoi(shlvl_string) + 1;
	(shlvl_string) ? free(shlvl_string) : 0;
	shlvl_string = ft_itoa(shlvl);
	shlvl_final = ft_strjoin("SHLVL=", shlvl_string);
	add_var_to_env(shlvl_final);
	(shlvl_string) ? free(shlvl_string) : 0;
	(shlvl_final) ? free(shlvl_final) : 0;
	return (1);
}

static int		beg_pwd(char **env)
{
	char *pwd;
	char *forfree;

	pwd = NULL;
	forfree = NULL;
	if (!get_pwd(&g_pwd))
		return (ERROR);
	dup_double_char_tab(env, &g_envp);
	sigcatcher_init();
	if (bool_get_env_var_by_name("OLDPWD") == 0)
		add_var_to_env("OLDPWD");
	pwd = ft_strdup("PWD=");
	forfree = pwd;
	pwd = ft_strjoin(pwd, g_pwd);
	(forfree) ? free(forfree) : 0;
	add_var_to_env(pwd);
	(pwd) ? free(pwd) : 0;
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

int				main(int ac, char **av, char **env)
{
	int			cmd_ret;
	char		*line;

	(void)ac;
	(void)av;
	line = NULL;
	if (beg_pwd(env) == ERROR)
		return (ERROR);
	if (isatty(0) && put_beg() && beg_shlvl())
	{
		while (42)
			if (minishell_loop(0, g_ms.entry, &cmd_ret) == ERROR)
				return (1);
	}
	else
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
