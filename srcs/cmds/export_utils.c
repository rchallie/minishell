/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/29 15:39:30 by thervieu          #+#    #+#             */
/*   Updated: 2020/09/29 15:39:31 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
** Function: is_set
** ------------
**		Check a variable is setted.
**
**		(char *)	var_name : variable to check.
**
**		returns: return 1 : if the variable is set.
**				 return 0 : if the variable is not set.
*/

int			is_set(
	char *var_name
)
{
	int		i;
	char	*end_name;

	i = 0;
	end_name = ft_strchr(var_name, '=');
	if (end_name == NULL)
		end_name = (var_name + ft_secure_strlen(var_name));
	while (g_export_vars && g_export_vars[i])
	{
		if (!ft_strncmp(g_export_vars[i], var_name, end_name - var_name))
			break ;
		i++;
	}
	if (g_export_vars && g_export_vars[i] != NULL
		&& g_export_vars[i][end_name - var_name] == '=')
		return (SUCCESS);
	return (ERROR);
}

/*
** Function: add_var_to_export
** ------------
**		Add/update a variable to export variables.
**
**		(char *)	var_name : variable to add.
**
**		returns: return 1 : if no problem
**				 return 0 : if an error appear
*/

int			add_var_to_export(char *var_name)
{
	int		i;
	int		plus;
	char	*end_name;

	i = 0;
	plus = 0;
	end_name = ft_strchr(var_name, '=');
	if (end_name)
		plus = (*(end_name - 1) == '+') ? 1 : 0;
	if (end_name == NULL && (is_set(var_name) == SUCCESS))
		return (SUCCESS);
	while (g_export_vars && g_export_vars[i] != NULL)
		if (!ft_strncmp(g_export_vars[i++],
			var_name, end_name - var_name - plus))
			break ;
	if (g_export_vars == NULL || g_export_vars[i] == NULL)
		return (add_word_to_tab(var_name, &g_export_vars));
	return (SUCCESS);
}

int			not_env(char *str)
{
	int		i;
	int		plus;
	char	*end_name;

	i = 0;
	plus = 0;
	end_name = ft_strchr(str, '=');
	if (end_name)
		plus = (*(end_name - 1) == '+') ? 1 : 0;
	i = 0;
	while (g_envp[i])
	{
		if (!ft_strncmp(g_envp[i], str, end_name - str - plus)
			&& g_envp[i][(end_name - plus - str)] == '=')
			return (0);
		i++;
	}
	return (1);
}

int			remove_from_export(char *str)
{
	int		i;
	int		plus;
	char	*end_name;

	i = 0;
	plus = 0;
	if (!g_export_vars)
		return (SUCCESS);
	end_name = ft_strchr(str, '=');
	if (end_name)
		plus = (*(end_name - 1) == '+') ? 1 : 0;
	i = 0;
	while (g_export_vars[i])
	{
		if (!ft_strncmp(g_export_vars[i], str, end_name - str - plus)
			&& (g_export_vars[i][(end_name - plus - str)] == '='
			|| g_export_vars[i][(end_name - plus - str)] == '\0'))
			if (double_char_tab_remove(&g_export_vars[i],
				&g_export_vars) == ERROR)
				return (ERROR);
		i++;
	}
	return (SUCCESS);
}

int			is_valid_name(char *str)
{
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	str++;
	while (*str && *str != '=')
	{
		if (!ft_isalnum(*str) && *str != '_')
			if (!(*str == '+' && *(str + 1) && *(str + 1) == '='))
				return (0);
		str++;
	}
	return (1);
}
