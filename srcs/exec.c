/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 14:34:30 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/19 18:59:07 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

/*
** Function : init_exec
** -------------------------
**		Init all values need for exec binaries
**
**		(t_exec *)ex :	a pointer to the exec structure at init
**
**		returns:	null
*/

static void		init_exec(t_exec *ex)
{
	ex->exec = NULL;
	ex->exec_path = NULL;
	ex->env_path = NULL;
	ex->path_list = NULL;
	ex->argv = NULL;
	ex->save_seq_cursor = g_ms.seq_cursor;
}

/*
** Function : exec_cmd
** -------------------------
**		Execute binarie from path was file
**
**		(char *)file		:	path to binarie
**		(t_exec *)ex		:	a pointer to the exec structure
**
**		returns:	return 0 :	if the binarie file was'nt found
**					return 1 :	if the binaries was sucessfull executed
*/

static int		exec_cmd(char *file, t_exec *ex)
{
	pid_t	pid;
	int		ret;
	int		status;
	char	*execute_env_path;

	pid = 0;
	if ((ret = open(file, O_RDONLY)) > 0)
	{
		execute_env_path = ft_strjoin("_=", file);
		add_var_to_env(execute_env_path);
		free(execute_env_path);
		if ((pid = fork()) == 0)
		{
			if ((execve(file, ex->argv, g_envp)) == -1)
				exit(errno);
		}
		else
		{
			waitpid(pid, &status, 0);
			g_ms.last_cmd_rtn = WEXITSTATUS(status);
			free(ex->argv);
			return (SUCCESS);
		}
	}
	return (ERROR);
}

/*
** Function : init_for_exec
** -------------------------
**		Initialise (t_exec *)ex (like argv) to be used command for absolute path
**
**		(t_exec *)ex		:	a pointer to the exec structure
**
**		returns:	return 0 :	if the malloc of argv array return NULL
**					return 1 :	if everything was okay
*/

static int		init_for_exec(t_exec *ex)
{
	char	*last_exec_path;

	ex->exec = g_ms.treated[ex->save_seq_cursor];
	add_word_to_tab(g_ms.treated[ex->save_seq_cursor], &ex->argv);
	if (g_ms.sequence[++ex->save_seq_cursor] > 2)
	{
		ex->argv ? free(ex->argv) : 0;
		if (!(ex->argv = (char **)malloc(sizeof(char *) * 2)))
			return (ERROR);
		ft_bzero(ex->argv, sizeof(char *) * 2);
		ex->argv[0] = "";
		ex->argv[1] = NULL;
	}
	else
		while (g_ms.sequence[ex->save_seq_cursor]
			&& g_ms.sequence[ex->save_seq_cursor] <= 2)
			add_word_to_tab(g_ms.treated[ex->save_seq_cursor++], &ex->argv);
	get_pwd(&ex->exec_path);
	last_exec_path = ex->exec_path;
	ex->exec_path = add_char_to_word(ex->exec_path, '/');
	free(last_exec_path);
	last_exec_path = ex->exec_path;
	ex->exec_path = ft_strjoin(ex->exec_path, ex->exec);
	free(last_exec_path);
	return (SUCCESS);
}

/*
** Function : exec_from_env
** -------------------------
**		Will try to execute binaries with path string stockedd in environement
**		variable PATH, exclude /bin && /usr/bin
**
**		(t_exec *)ex		:	a pointer to the exec structure
**
**		returns:	return 0 :	if the malloc of argv array return NULL
**					return 1 :	if everything was okay
*/

static int		exec_from_env(t_exec *ex, int i, char *last_exec_path)
{
	ex->env_path = get_env_var_by_name("PATH");
	ex->path_list = ft_split(ex->env_path, ':');
	free(ex->env_path);
	if (is_cmd(g_ms.treated[g_ms.seq_cursor]) == -1)
	{
		while (++i < get_double_char_tab_len(ex->path_list))
		{
			ex->exec_path = add_char_to_word(ex->path_list[i], '/');
			last_exec_path = ex->exec_path;
			ex->exec_path = ft_strjoin(ex->exec_path, ex->exec);
			free(last_exec_path);
			if (exec_cmd(ex->exec_path, ex) == SUCCESS)
			{
				free_double_char_tab(ex->path_list);
				free(ex->exec_path);
				return (SUCCESS);
			}
		}
	}
	free(ex->exec_path);
	ex->exec_path = NULL;
	free_double_char_tab(ex->path_list);
	return (ERROR);
}

/*
** Function : is_exec
** -------------------------
**		Will try to execute binaries where seq_cursor was
**
**		returns:	return 0 :	if init as a problem, if len of the binarie name
**								is NULL or is the binaries was not found
**					return 1 :	if the binarie was found and executed
*/

int				is_exec(void)
{
	t_exec	ex;

	init_exec(&ex);
	if (!init_for_exec(&ex))
		return (ERROR);
	if (ft_secure_strlen(ex.exec) == 0)
		return (ERROR);
	if (exec_cmd(ex.exec, &ex) == SUCCESS)
		return (SUCCESS);
	free(ex.exec_path);
	ex.exec_path = NULL;
	if (exec_from_env(&ex, -1, NULL) == SUCCESS)
		return (SUCCESS);
	free(ex.argv);
	return (ERROR);
}
