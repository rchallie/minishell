/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 13:13:15 by thervieu          #+#    #+#             */
/*   Updated: 2020/04/25 12:13:46 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int		print_export_vars(char **export_vars)
{
	int		i;
	char	**sorted;

	i = 0;
	if (!dup_double_char_tab(export_vars, &sorted))
		return (ERROR);
	sorted = double_tab_bubble_sort(&sorted);
	while (sorted && sorted[i])
	{
		if (ft_strchr(sorted[i], '=') != NULL)
			ft_printf("declare -x %.*s\"%s\"\n", (ft_strchr(sorted[i], '=')
				- sorted[i]) + 1, sorted[i], ft_strchr(sorted[i], '=') + 1);
		else
			ft_printf("declare -x %.*s\n", (ft_strchr(sorted[i], '=')
				- sorted[i]) + 1, sorted[i]);
		i++;
	}
	free_double_char_tab(sorted);
	return (SUCCESS);
}

static int		export(char **export_vars)
{
	int		i;
	char	**sorted;

	i = 0;
	if (!dup_double_char_tab(envp, &sorted))
		return (ERROR);
	sorted = double_tab_bubble_sort(&sorted);
	while (sorted && sorted[i])
	{
		if (double_char_tab_contain(sorted[i], export_vars) == ERROR
			&& ft_strncmp(sorted[i], "_", (ft_strchr(sorted[i], '=')
				- sorted[i])))
			ft_printf("declare -x %.*s\"%s\"\n", (ft_strchr(sorted[i], '=')
				- sorted[i]) + 1, sorted[i], ft_strchr(sorted[i], '=') + 1);
		i++;
	}
	free_double_char_tab(sorted);
	return (print_export_vars(export_vars));
}

static int		add_var_to_export(char *var, char ***export_vars)
{
	int		i;
	char	*end_name;

	i = 0;
	end_name = ft_strchr(var, '=');
	while (*export_vars && (*export_vars)[i] != NULL)
	{
		if (!ft_strncmp((*export_vars)[i], var, end_name - var))
			break ;
		i++;
	}
	if (*export_vars == NULL || (*export_vars)[i] == NULL)
		return (add_word_to_tab(var, export_vars));
	else
	{
		ft_strdel((export_vars)[i]);
		(*export_vars)[i] = ft_strdup(var);
	}
	return (SUCCESS);
}

static int		add_var_to_env(char *var)
{
	int		i;
	char	*end_name;

	i = 0;
	end_name = ft_strchr(var, '=');
	while (envp[i] != NULL)
	{
		if (!ft_strncmp(envp[i], var, end_name - var)
			&& envp[i][(end_name - var)] == '=')
			break ;
		i++;
	}
	if (envp[i] == NULL)
		return (add_word_to_tab(var, &envp));
	else
	{
		ft_strdel(&envp[i]);
		envp[i] = ft_strdup(var);
	}
	return (SUCCESS);
}

int				export_(int argc, char **argv, char **envp)
{
	static char	**export_vars;
	int			cursor;
	char		*end_name;

	(void)argc;
	(void)envp;
	cursor = 0;
	if (argc > 1)
	{
		while (argv[++cursor])
		{
			end_name = ft_strchr(argv[cursor], '=');
			if (end_name == argv[cursor])
			{
				ft_printf("minishell: export: « %s » : %s\n",
					argv[cursor], "invalid identifier");
				continue;
			}
			else if (end_name != NULL)
				add_var_to_env(argv[cursor]);
			add_var_to_export(argv[cursor], &export_vars);
		}
	}
	else
		export(export_vars);
	return (0);
}
