/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 13:13:15 by thervieu          #+#    #+#             */
/*   Updated: 2020/09/16 19:01:31 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
** Function: print_g_export_vars
** ------------
**		Print the list of commands to export "export variables".
**
**		returns: return 1 : if no problem
**				 return 0 : if an error appear
*/

static int		print_g_export_vars(void)
{
	int		i;
	char	**sorted;

	i = 0;
	if (!dup_double_char_tab(g_export_vars, &sorted))
		return (ERROR);
	sorted = double_tab_bubble_sort(&sorted);
	while (sorted && sorted[i])
	{
		if (ft_strchr(sorted[i], '=') != NULL)
			ft_printf(STDOUT_FILENO, "declare -x %.*s\"%s\"\n",
				(ft_strchr(sorted[i], '=') - sorted[i]) + 1, sorted[i],
				ft_strchr(sorted[i], '=') + 1);
		else
			ft_printf(STDOUT_FILENO, "declare -x %.*s\n",
				(ft_strchr(sorted[i], '=') - sorted[i]) + 1, sorted[i]);
		i++;
	}
	free_double_char_tab(sorted);
	return (SUCCESS);
}

/*
** Function: export
** ------------
**		Print the list of commands to export environement variables
**		(exepted path to actual binari executed) and call
**		for print export variables.
**
**		returns: return 1 : if no problem
**				 return 0 : if an error appear
*/

static int		export(void)
{
	int		i;
	char	**sorted;

	i = 0;
	if (!dup_double_char_tab(g_envp, &sorted))
		return (ERROR);
	sorted = double_tab_bubble_sort(&sorted);
	while (sorted && sorted[i])
	{
		if (double_char_tab_contain(sorted[i], g_export_vars) == ERROR
			&& ft_strncmp(sorted[i], "_", (ft_strchr(sorted[i], '=')
				- sorted[i])))
			(ft_strchr(sorted[i], '=') == NULL) ? ft_printf(STDOUT_FILENO, "declare -x %.*s\n",
				(ft_strchr(sorted[i], '=') - sorted[i]) + 1) :(ft_printf(STDOUT_FILENO, "declare -x %.*s\"%s\"\n",
				(ft_strchr(sorted[i], '=') - sorted[i]) + 1, sorted[i],
				ft_strchr(sorted[i], '=') + 1));
		i++;
	}
	free_double_char_tab(sorted);
	return (print_g_export_vars());
}

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

static int		is_set(
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

static int		add_var_to_export(
	char *var_name
)
{
	int		i;
	int		plus;
	char	*end_name;

	i = 0;
	plus = 0;
	end_name = ft_strchr(var_name, '=');
	if (end_name)
		plus = (*(end_name - 1) == '+') ? 1 : 0;
	ft_printf(1, "plus = |%d|\n", plus);
	if (end_name == NULL && (is_set(var_name) == SUCCESS))
		return (SUCCESS);
	while (g_export_vars && g_export_vars[i] != NULL)
	{
		if (!ft_strncmp(g_export_vars[i], var_name, end_name - var_name - plus))
			break ;
		i++;
	}
	if (g_export_vars == NULL || g_export_vars[i] == NULL)
		return (add_word_to_tab(var_name, &g_export_vars));
	else
	{
		if (*(end_name - 1) == '+')
			g_export_vars[i] = ft_strjoin(g_export_vars[i], end_name);
		else
		{
			ft_strdel(&g_export_vars[i]);
			g_export_vars[i] = ft_strdup(var_name);
		}
	}
	return (SUCCESS);
}

int				not_env(char *str)
{
	int i;
	int plus;
	char *end_name;

	i = 0;
	plus = 0;
	end_name = ft_strchr(str, '=');
	if (end_name)
		plus = (*(end_name - 1) == '+') ? 1 : 0;
	i = 0;
	while (g_envp[i])
	{
		if (!ft_strncmp(g_envp[i], str, end_name - str - plus)
			&& g_envp[i][(end_name - str)] == '=')
		{
			return (0);
		}
		i++;
	}
	return (1);
}

/*
** Function: export_
** ------------
**		(Reproduction of the real export command).
** 		Print the list of command to export easly environement
**	 	variables.
**		Permeted to add new environement variable that depends of
**		the actual minishell used.
**
**		(int)		argc : number of arguments of the command line.
**		(char **)	argv : arguments array.
**		(char **)	ms.envp : environement variable array.
**
**		returns: return 0
*/

int				is_valid_name(char *str)
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

int				export_(
	int argc,
	char **argv,
	char **envp
)
{
	(void)envp;
	int			cursor;
	char		*end_name;
	int			error;

	error = 0;
	cursor = 0;
	if (argc > 1)
	{
		while (argv[++cursor])
		{
			if (is_valid_name(argv[cursor]) == 0)
			{
				ft_printf(2, "minishell: export: '%s': %s",
					argv[cursor], "not a valid identifier\n");
				error++;
			}
			else
			{
				end_name = ft_strchr(argv[cursor], '=');
				if (end_name == argv[cursor])
				{
					ft_printf(2, "minishell: export: « %s » : %s\n",
						argv[cursor], "not a valid identifier");
				}
				else if (end_name != NULL)
				{
					add_var_to_env(argv[cursor]);
				}
				if (not_env(argv[cursor]))
					add_var_to_export(argv[cursor]);
			}
		}
	}
	else
		export();
	if (error == 0)
		return (0);
	else
		return (1);
}
