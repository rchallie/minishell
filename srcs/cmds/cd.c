/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 13:36:32 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/22 16:53:56 by excalibur        ###   ########.fr       */
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
	if (!argv[cursor + 1]
		|| argv[cursor + 1][0] == '~')
	{
		path = get_env_var_by_name("home", envp);
		if (argv[cursor + 1])
		{
			path = ft_strjoin(path,
				ft_strdup(&argv[cursor + 1][1]));
			cursor++;
		}
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
		
		chdir_return = chdir(path);
		if (chdir_return == -1)
			return (error_path("cd", argv[cursor + 1], errno));
	}
	return (SUCCESS);
}
