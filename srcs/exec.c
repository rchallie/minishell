/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 14:34:30 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/16 14:11:13 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

static int		do_cmd(char *file, t_exec *ex)
{
	pid_t	pid;
	int		status;
	int		pipe_[2];

	pid = 0;
	//ADD CHECK MALLOC
	if (pipe(pipe_) == -1)
		exit(2);
	if ((pid = fork()) == 0)
	{
		close(pipe_[0]);
		dup2(pipe_[1], 2);
		close(pipe_[1]);
		if ((execve(file, ex->argv, g_envp)) == -1)
			exit(errno);
	}
	else
	{
		close(pipe_[1]);
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
	char	*last_exec_path;

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

static int		exec_from_env(t_exec *ex, int i, char *last_exec_path, char **cmd)
{
	ex->env_path = get_env_var_by_name("PATH");
	// if (ex->env_path)

	// char *new_start = ex->env_path;
	// char *find = NULL;
	// char *path;
	// while (new_start)
	// {
	// 	path = NULL;
	// 	find = ft_strchr(new_start, ':');
	// 	if (!find)
	// 		find = new_start + ft_secure_strlen(new_start);
	// 	path = ft_substr(ex->env_path, new_start - ex->env_path, find - new_start);
	// 	add_word_to_tab(path, &ex->path_list);
	// 	exit(1);
	// 	if (path)
	// 		free(path);
	// 	if (find)
	// 		new_start = find + 1;
	// 	else
	// 		new_start = find;
	// }

	ex->path_list = ft_split(ex->env_path, ':');

	free(ex->env_path);
	if (is_cmd(cmd[0]) == -1)
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
			free(ex->exec_path);
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
		free(ex.exec_path);
		free_double_char_tab(ex.argv);
		return (ERROR);
	}
	if (exec_cmd(ex.exec, &ex) == SUCCESS)
		return (SUCCESS);
	free(ex.exec_path);
	ex.exec_path = NULL;
	if (exec_from_env(&ex, -1, NULL, cmd) == SUCCESS)
		return (SUCCESS);
	free_double_char_tab(ex.argv);
	return (ERROR);
}
