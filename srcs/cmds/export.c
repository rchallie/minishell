/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 13:13:15 by thervieu          #+#    #+#             */
/*   Updated: 2020/05/13 16:22:34 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
** Function: print_export_vars
** ------------
**		Print the list of commands to export "export variables".
**
**		returns: return 1 : if no problem
**				 return 0 : if an error appear
*/

static int		print_export_vars(void)
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
	return (print_export_vars());
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
	while (export_vars && export_vars[i])
	{
		if (!ft_strncmp(export_vars[i], var_name, end_name - var_name))
			break ;
		i++;
	}
	if (export_vars && export_vars[i] != NULL
		&& export_vars[i][end_name - var_name] == '=')
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
	char	*end_name;

	i = 0;
	end_name = ft_strchr(var_name, '=');
	if (end_name == NULL && (is_set(var_name) == SUCCESS))
		return (SUCCESS);
	while (export_vars && export_vars[i] != NULL)
	{
		if (!ft_strncmp(export_vars[i], var_name, end_name - var_name))
			break ;
		i++;
	}
	if (export_vars == NULL || export_vars[i] == NULL)
		return (add_word_to_tab(var_name, &export_vars));
	else
	{
		ft_strdel(&export_vars[i]);
		export_vars[i] = ft_strdup(var_name);
	}
	return (SUCCESS);
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

int				export_(
	int argc,
	char **argv,
	char **envp
)
{
	int			cursor;
	char		*end_name;

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
			add_var_to_export(argv[cursor]);
		}
	}
	else
		export();
	return (0);
}
