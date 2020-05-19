/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:48:15 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/19 19:01:36 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

char		*get_env_var_by_name(char *name)
{
	int		i;
	char	*rename;
	int		j;

	rename = ft_strdup(name);
	i = 0;
	while (g_envp[i])
	{
		j = 0;
		while (g_envp[i][j] && rename[j] && g_envp[i][j] == rename[j])
			j++;
		if (rename[j] == '\0' && g_envp[i][j] == '=')
		{
			j++;
			free(rename);
			return (ft_strdup(&g_envp[i][j]));
		}
		i++;
	}
	if (rename)
		free(rename);
	return (ft_strdup(""));
}

int			remove_var_env(char *var_name)
{
	int		i;
	char	*end_name;

	i = 0;
	while (g_envp[i])
	{
		end_name = ft_strchr(g_envp[i], '=');
		if (!ft_strncmp(g_envp[i], var_name, end_name - g_envp[i]))
			break ;
		i++;
	}
	if (g_envp[i] != NULL)
		return (double_char_tab_remove(&g_envp[i], &g_envp));
	return (SUCCESS);
}

/*
** Function: add_var_to_env
** ------------
**		Add/update a variable to environement variable if is form
**		is <name>=<variable>
**
**		(char *)	var : variable to add.
**
**		returns: return 1 : if no problem
**				 return 0 : if an error appear
*/

int			add_var_to_env(char *var)
{
	int		i;
	char	*end_name;

	i = 0;
	end_name = ft_strchr(var, '=');
	while (g_envp[i] != NULL)
	{
		if (!ft_strncmp(g_envp[i], var, end_name - var)
			&& g_envp[i][(end_name - var)] == '=')
			break ;
		i++;
	}
	if (g_envp[i] == NULL)
		return (add_word_to_tab(var, &g_envp));
	else
	{
		ft_strdel(&g_envp[i]);
		g_envp[i] = ft_strdup(var);
	}
	return (SUCCESS);
}

int			env(int argc, char **argv, char **g_envp)
{
	char	*env_list;
	int		cursor;
	int		i;

	(void)argc;
	(void)argv;
	i = 0;
	cursor = 0;
	env_list = NULL;
	while (g_envp[i])
	{
		env_list = ft_strjoin(env_list, g_envp[i]);
		env_list = add_char_to_word(env_list, '\n');
		i++;
	}
	ft_printf(STDOUT_FILENO, "%s", env_list);
	return (0);
}
