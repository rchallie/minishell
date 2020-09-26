/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:48:15 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/26 17:47:54 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"


int			bool_get_env_var_by_name(char *name)
{
	int		i;
	int		j;

	i = 0;
	while (g_envp[i])
	{
		j = 0;
		while (g_envp[i][j] && name[j] && g_envp[i][j] == name[j])
			j++;
		if (name[j] == '\0' && g_envp[i][j] == '=')
		{
			j++;
			return (1);
		}
		i++;
	}
	return (0);
}

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
		if (end_name == NULL)
			end_name = (g_envp[i] + ft_secure_strlen(g_envp[i]));
		if (!ft_strncmp(g_envp[i], var_name, ft_strlen(var_name)))
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
	int		plus = 0;
	char	*end_name;

	i = 0;
	end_name = ft_strchr(var, '=');
	if (end_name)
		plus = (*(end_name - 1) == '+') ? 1 : 0;
	// ft_printf(1, "VAR = |%.*s|\n", end_name - var - plus, var);
	while (g_envp[i] != NULL)
	{
		if (!ft_strncmp(g_envp[i], var, end_name - var - plus)
			&& g_envp[i][((end_name - plus) - var)] == '=')
			break ;
		i++;
	}
	if (g_envp[i] == NULL)
	{
		return (add_word_to_tab(var, &g_envp));
	}
	else
	{
		//ft_printf(1, "ENV existe\n");
		if (*(end_name - 1) == '+')
			g_envp[i] = ft_strjoin(g_envp[i], end_name + 1);
		else
		{
			ft_strdel(&g_envp[i]);
			g_envp[i] = ft_strdup(var);
		}
	}
	return (SUCCESS);
}

int			env(int argc, char **argv, char **g_envp)
{
	char	*env_list;
	int		cursor;
	int		i;
	char	*save;

	(void)argc;
	(void)argv;
	i = 0;
	cursor = 0;
	env_list = NULL;
	if (argc >= 2)
		return (2);
	while (g_envp[i])
	{
		save = env_list;
		env_list = ft_strjoin(env_list, g_envp[i]);
		(save) ? free(save) : 0;
		env_list = add_char_to_word_free(env_list, '\n');
		i++;
	}
	ft_printf(STDOUT_FILENO, "%s", env_list);
	(env_list) ? free(env_list) : 0;
	return (0);
}
