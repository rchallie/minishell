/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:36:32 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/28 17:13:09 by excalibur        ###   ########.fr       */
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
	// check par rapport a la séquence
	// If > (empty) error newline (bash: syntax error near unexpected token `newline')
	// if | spec error (bash: syntax error near unexpected token `|')
	// check if treated[i + 1] est logic ou est un special
	// printf("cd : %s\n", treated[i + 1]);
	// printf("home : %s\n", get_env_var_by_name("home", envp));
	(void)argc;
	cursor = 0;
	if (!argv[cursor + 1])
	{
		path = get_env_var_by_name("HOME", envp);
		if (!argv[cursor + 1] && ft_secure_strlen(path) == 0)
			return (error_unidentified("cd", "HOME"));
	}
	else
		path = ft_strdup(argv[cursor + 1]);
	dir = opendir(path);
	if (!dir)
		return	 (error_path("cd", argv[cursor + 1], errno));
	else
	{
		closedir(dir);
		// treat_output
		// int ret_treat = 0;
		// int i = 0;
		// while (!(ms->sequence[i] >= 3 && ms->sequence[i] <= 7))
		// 	i++;
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
