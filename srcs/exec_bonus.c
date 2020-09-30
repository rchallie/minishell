/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 14:34:30 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/30 18:12:06 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static int		do_cmd(char *file, t_exec *ex)
{
	pid_t	pid;
	int		status;

	pid = 0;
	if ((pid = fork()) == 0)
	{
		if ((execve(file, ex->argv, g_envp)) == -1)
			exit(errno);
	}
	else
	{
		waitpid(pid, &status, 0);
		g_ms.last_cmd_rtn = WEXITSTATUS(status);
		free_double_char_tab(ex->argv);
		return (SUCCESS);
	}
	return (ERROR);
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
	int			ret;
	char		*execute_env_path;
	struct stat	sb;

	if ((ret = open(file, O_RDONLY)) > 0)
	{
		close(ret);
		if (!(stat(file, &sb) == 0 && sb.st_mode & S_IXUSR)
			|| S_ISDIR(sb.st_mode))
			return (ERROR);
		execute_env_path = ft_strjoin("_=", file);
		add_var_to_env(execute_env_path);
		free(execute_env_path);
		return (do_cmd(file, ex));
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

static int		init_for_exec(t_exec *ex, char **cmd, int *seq)
{
	ex->exec = cmd[ex->save_seq_cursor];
	add_word_to_tab(cmd[ex->save_seq_cursor], &ex->argv);
	if (seq[++ex->save_seq_cursor] > 2)
	{
		ex->argv ? free_double_char_tab(ex->argv) : 0;
		if (!(ex->argv = (char **)malloc(sizeof(char *) * 2)))
			return (ERROR);
		ft_bzero(ex->argv, sizeof(char *) * 2);
		ex->argv[0] = ft_strdup("");
		ex->argv[1] = NULL;
	}
	else
		while (seq[ex->save_seq_cursor]
			&& seq[ex->save_seq_cursor] <= 2)
			add_word_to_tab(cmd[ex->save_seq_cursor++], &ex->argv);
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

static int		exec_from_env(
	t_exec *ex,
	int i,
	char *last_exec_path,
	char **cmd
)
{
	ex->env_path = get_env_var_by_name("PATH");
	ex->path_list = ft_split(ex->env_path, ':');
	free(ex->env_path);
	if (is_cmd(cmd[0]) == -1)
	{
		while (++i < get_double_char_tab_len(ex->path_list))
		{
			ex->exec_path = add_char_to_word(ex->path_list[i], '/');
			last_exec_path = ex->exec_path;
			ex->exec_path = ft_strjoin(ex->exec_path, ex->exec);
			(last_exec_path) ? free(last_exec_path) : 0;
			if (exec_cmd(ex->exec_path, ex) == SUCCESS)
			{
				free_double_char_tab(ex->path_list);
				free(ex->exec_path);
				return (SUCCESS);
			}
			(ex->exec_path) ? free(ex->exec_path) : 0;
		}
	}
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

int				is_exec(char **cmd, int *seq)
{
	t_exec	ex;

	ex.exec = NULL;
	ex.exec_path = NULL;
	ex.env_path = NULL;
	ex.path_list = NULL;
	ex.argv = NULL;
	ex.save_seq_cursor = 0;
	if (!init_for_exec(&ex, cmd, seq))
		return (ERROR);
	if (ft_secure_strlen(ex.exec) == 0)
	{
		(ex.exec_path) ? free(ex.exec_path) : 0;
		free_double_char_tab(ex.argv);
		return (ERROR);
	}
	if (exec_from_env(&ex, -1, NULL, cmd) == SUCCESS)
		return (SUCCESS);
	ex.exec_path = NULL;
	if (exec_cmd(ex.exec, &ex) == SUCCESS)
		return (SUCCESS);
	free_double_char_tab(ex.argv);
	return (ERROR);
}
