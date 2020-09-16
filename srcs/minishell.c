/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/16 17:33:11 by excalibur        ###   ########.fr       */
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

static int		entry_splitter_precheck(
	char *entry
)
{
	int s_quote = 0;
	int d_quote = 0;
	if (*entry && *entry == ';')
	{
		if (*(entry + 1) && *(entry + 1) == ';')
			ft_printf(2, "minishell : syntax error near unexpected token « ;; »\n");
		else
			ft_printf(2, "minishell : syntax error near unexpected token « ; »\n");
		return (ERROR);
	}
	while (*entry)
	{
		if (*entry == '\'' && *(entry - 1) != '\\' && s_quote == 0)
			s_quote = 1;
		else if (*entry == '\'' && *(entry - 1) != '\\' && s_quote == 1)
			s_quote = 0;
		if (*entry == '"' && *(entry - 1) != '\\' && d_quote == 0)
			d_quote = 1;
		else if (*entry == '"' && *(entry - 1) != '\\' && d_quote == 1)
			d_quote = 0;
		if ((s_quote != 1 && d_quote != 1) && *entry && *entry == ';' && *(entry + 1) == ';')
		{
			ft_printf(2, "minishell : syntax error near unexpected token « ;; »\n");
			return (ERROR);
		}
		entry++;
	}
	return (SUCCESS);
}

static void		entry_splitter(
	char *entry
)
{
	char *cmd;
	char *find = NULL;
	char *new_start = entry;

	if (entry_splitter_precheck(new_start) == ERROR)
	{
		g_ms.last_cmd_rtn = 2;
		return;
	}
	
	int s_quote = 0;
	int d_quote = 0;
	while (*new_start)
	{
		find = ft_strchr(new_start, ';');
		if (!find)
		{
			find = (new_start + ft_secure_strlen(new_start));
		}
		char *cp = new_start;
		while (*cp && cp != find)
		{
			if (*cp == '\'' && *(cp - 1) != '\\' && (s_quote == 0 && d_quote == 0))
				s_quote = 1;
			else if (*cp == '\'' && s_quote == 1)
				s_quote = 0;
			if (*cp == '"' && *(cp - 1) != '\\' && (d_quote == 0 && s_quote == 0))
				d_quote = 1;
			else if (*cp == '"' && *(cp - 1) != '\\' && d_quote == 1)
				d_quote = 0;
			if (cp && *(cp + 1) && (cp + 1) == find && (s_quote == 1 || d_quote == 1))
				if (*find && (*find + 1))
					find = ft_strchr(find + 1, ';');
			cp++;
		}
		if (!find)
			find = (new_start + ft_secure_strlen(new_start));
		cmd = ft_substr(entry, new_start - entry, find - new_start);
		treat_entry(cmd);
		s_quote = 0;
		d_quote = 0;
		if (find && *find && *(find + 1) != 0)
			new_start = find + 1;
		else
			new_start = 0;
		if (new_start == 0)
			break ;
	}

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
	int			rtn;
	char		*line;

	(void)ac;
	(void)av;
	cmd_ret = 0;
	rtn = 0;

	if(!get_pwd(&g_pwd))
		return (ERROR);
	dup_double_char_tab(env, &g_envp);
	sigcatcher_init();
	
	char *shlvl_string = get_env_var_by_name("SHLVL");
	int shlvl = ft_atoi(shlvl_string);
	free(shlvl_string);
	shlvl_string = ft_itoa(shlvl + 1);
	char *shlvl_final = ft_strjoin("SHLVL=",shlvl_string);
	add_var_to_env(shlvl_final);
	free(shlvl_string);
	free(shlvl_final);

	if (isatty(0))
	{
		put_beg();
		while (42)
			if (minishell_loop(0, g_ms.entry, &cmd_ret) == ERROR)
				return (1);
	}
	else
		while ((rtn = get_next_line(0, &line)) > 0)
		{
			//ft_printf(1, "line = |%s|\n", line);
			if (minishell_loop(1, line, &cmd_ret) == ERROR)
				return (1);
			cmd_ret = g_ms.last_cmd_rtn;
		}
	return (g_ms.last_cmd_rtn);
}
