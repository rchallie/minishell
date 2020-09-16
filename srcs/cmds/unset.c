/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/25 17:51:16 by excalibur         #+#    #+#             */
/*   Updated: 2020/09/16 19:42:44 by excalibur        ###   ########.fr       */
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

static int	remove_var_export(
	char *var_name
)
{
	int		i;
	char	*end_name;

	i = 0;
	while (g_export_vars[i])
	{
		end_name = ft_strchr(g_export_vars[i], '=');
		if (end_name == NULL)
			end_name = (g_export_vars[i] + ft_secure_strlen(g_export_vars[i]));
		if (!ft_strncmp(g_export_vars[i],
			var_name, ft_strlen(var_name)))
			break ;
		i++;
	}
	if (g_export_vars[i] != NULL)
		return (double_char_tab_remove(&g_export_vars[i], &g_export_vars));
	return (SUCCESS);
}

/*
** Function: unset
** ------------
**		Unset a variable from environement and export variables.
**
**		(int)		argc : number of arguments of the command line.
**		(char **)	argv : arguments array.
**		(char **)	g_envp : environement variable array.
**
**		returns: return 0;
*/

int			unset(
	int argc,
	char **argv,
	char **env
)
{
	int cursor;

	(void)argc;
	(void)env;
	cursor = 0;
	while (argv[++cursor])
	{
		if (g_envp)
			remove_var_env(argv[cursor]);
		if (g_export_vars)
			remove_var_export(argv[cursor]);
	}
	return (0);
}
