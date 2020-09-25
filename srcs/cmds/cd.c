/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:36:32 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/26 01:18:38 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../../incs/minishell.h"

/*
**	Function: change_dir
**	--------------------
**		Change working directory.
**
**		(char *)	path : final path to directory.
**		(int)		curosr : arguments cursor.
**		(char **)	argv : arguments.
**
**		returns:	return 0 : no problem.
**					return 1 : an error appear.
*/

static int	change_dir(
	char *path,
	int cursor,
	char **argv
)
{
	DIR			*dir;
	int			rtn;
	char		*old_pwd;

	dir = opendir(path);
	if (!dir)
		return (error_path("cd", argv[cursor + 1], errno));
	else
	{
		closedir(dir);
		old_pwd = get_env_var_by_name("PWD");
		char *to_free = ft_strjoin("OLDPWD=", old_pwd);
		(old_pwd) ? free(old_pwd) : 0;
		add_var_to_env(to_free);
		(to_free) ? free(to_free) : 0;
		rtn = chdir(path);
		to_free = g_pwd;
		if (get_pwd(&g_pwd) == ERROR_NO_CURRENT_WORK_DIR
			&& (!ft_strcmp(".", path) || !ft_strcmp("./", path)))
		{
			ft_printf(2, "cd: error retrieving current directory: getcwd: cannot access parent directories: %s\n", strerror(errno));
			g_pwd = add_char_to_word(g_pwd, '/');
			old_pwd = g_pwd;
			g_pwd = ft_strjoin(g_pwd, path);
			(old_pwd) ? free(old_pwd) : 0;
		}
		(to_free) ? free(to_free) : 0;
		if (rtn == -1)
			return (error_path("cd", argv[cursor + 1], errno));
	}
	return (0);
}

/*
**	Function: cd
**	------------
** 		Check for "HOME" variable in argument given
**	in command line, call for change directory, and
**	update pwd environnement variable.
**
**		(int)		argc : argument count.
**		(char **)	argv : arguments.
**		(char **)	envp : environment variables.
**
**		returns: 	return 0 : no problem.
**					return 1 : an error appear.
*/

int			cd(
	int argc,
	char **argv,
	char **envp
)
{
	char		*path;
	int			cursor;
	char		*pwd;

	(void)argc;
	(void)envp;
	cursor = 0;
	if (argc > 2)
	{
		ft_printf(2, "minishell: cd: to many arguments\n");
		return (1);
	}
	if (!argv[cursor + 1])
	{
		path = get_env_var_by_name("HOME");
		if (!argv[cursor + 1] && ft_secure_strlen(path) == 0)
		{
			(path) ? free(path) : 0;
			return (error_unidentified("cd", "HOME"));
		}
	}
	else
		path = ft_strdup(argv[cursor + 1]);
	if (change_dir(path, cursor, argv) != 0)
	{
		(path) ? free(path) : 0;
		return (1);
	}
	get_shell_pwd(&pwd);
	char *pwd_env = ft_strjoin("PWD=", pwd);
	add_var_to_env(pwd_env);
	(path) ? free(path) : 0;
	(pwd) ? free(pwd) : 0;
	(pwd_env) ? free(pwd_env) : 0;
	return (0);
}
