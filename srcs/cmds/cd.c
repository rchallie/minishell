/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:36:32 by rchallie          #+#    #+#             */
/*   Updated: 2020/08/11 18:06:20 by excalibur        ###   ########.fr       */
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
	int			chdir_return;
	char		*old_pwd;

	dir = opendir(path);
	if (!dir)
		return (error_path("cd", argv[cursor + 1], errno));
	else
	{
		closedir(dir);
		if (get_pwd(&old_pwd) == ERROR)
		{
			ft_printf(2, "cd : %s\n", strerror(errno));
			return (1);
		}
		add_var_to_env(ft_strjoin("OLDPWD=", old_pwd));
		chdir_return = chdir(path);
		if (chdir_return == -1)
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
	if (!argv[cursor + 1])
	{
		path = get_env_var_by_name("HOME");
		if (!argv[cursor + 1] && ft_secure_strlen(path) == 0)
			return (error_unidentified("cd", "HOME"));
	}
	else
		path = ft_strdup(argv[cursor + 1]);
	if (change_dir(path, cursor, argv) != 0)
		return (1);
	if (get_pwd(&pwd) == ERROR)
		return (1);
	add_var_to_env(ft_strjoin("PWD=", pwd));
	return (0);
}
