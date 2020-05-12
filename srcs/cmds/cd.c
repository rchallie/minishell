/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:36:32 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/05 14:45:48 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../libft/libft.h"
# include "../../incs/minishell.h"

/*
** Function: cd
** ------------
** 		Change working directory
**
**		(t_minishell *)ms : minishell global variables
**
**		returns: return 1
*/

int		cd(int argc, char **argv, char **envp)
{
	int			chdir_return;
	extern int	errno;
	char		*path;
	int cursor;
	DIR			*dir;

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
	dir = opendir(path);
	if (!dir)
		return (error_path("cd", argv[cursor + 1], errno));
	else
	{
		closedir(dir);
		char *old_pwd;
		if (get_pwd(&old_pwd) == ERROR)
			return (1);
		add_var_to_env(ft_strjoin("OLDPWD=", old_pwd));
		chdir_return = chdir(path);
		if (chdir_return == -1)
			return (error_path("cd", argv[cursor + 1], errno));
	}
	char *pwd;
	if (get_pwd(&pwd) == ERROR)
		return (1);
	add_var_to_env(ft_strjoin("PWD=", pwd));
	return (0);
}
