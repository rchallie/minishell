/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/25 17:51:16 by excalibur         #+#    #+#             */
/*   Updated: 2020/05/02 12:24:19 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
** Function: remove_var_export
** ------------
**		Remove a variable from export variables.
**
**		(char *)	var_name : variable name to remove.
**
**		returns: return 1 : if no problem
**				 return 0 : if an error appear
*/

static int	remove_var_export(char *var_name)
{
	int		i;
	char	*end_name;

	i = 0;
	while (export_vars[i])
	{
		end_name = ft_strchr(export_vars[i], '=');
		if (end_name == NULL)
			end_name = (export_vars[i] + ft_secure_strlen(export_vars[i]));
		if (!ft_strncmp(export_vars[i], var_name, end_name - export_vars[i]))
			break ;
		i++;
	}
	if (export_vars[i] != NULL)
		return (double_char_tab_remove(&export_vars[i], &export_vars));
	return (SUCCESS);
}

/*
** Function: unset
** ------------
**		Unset a variable from environement and export variables.
**
**		(int)		argc : number of arguments of the command line.
**		(char **)	argv : arguments array.
**		(char **)	envp : environement variable array.
**
**		returns: return 0;
*/

int			unset(int argc, char **argv, char **env)
{
	int cursor;

	(void)argc;
	(void)env;
	cursor = 0;
	while (argv[++cursor])
	{
		if (envp)
			remove_var_env(argv[cursor]);
		if (export_vars)
			remove_var_export(argv[cursor]);
	}
	return (0);
}
