/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/09 13:13:15 by thervieu          #+#    #+#             */
/*   Updated: 2020/09/26 22:30:08 by excalibur        ###   ########.fr       */
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
			(ft_strchr(sorted[i], '=') == NULL) ? ft_printf(STDOUT_FILENO,
			"declare -x %.*s\n", (ft_strchr(sorted[i], '=') - sorted[i]) + 1)
			: (ft_printf(STDOUT_FILENO, "declare -x %.*s\"%s\"\n",
			(ft_strchr(sorted[i], '=') - sorted[i]) + 1, sorted[i],
			ft_strchr(sorted[i], '=') + 1));
		i++;
	}
	free_double_char_tab(sorted);
	return (print_g_export_vars());
}

static int		is_valid_name(char *str)
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

static void		if_valid_name(
	int *error,
	char **argv,
	int cursor
)
{
	char		*end_name;

	end_name = NULL;
	end_name = ft_strchr(argv[cursor], '=');
	if (end_name == argv[cursor])
	{
		ft_printf(2, "minishell: export: « %s » : %s\n",
			argv[cursor], "not a valid identifier");
		(*error)++;
	}
	else if (end_name != NULL)
		add_var_to_env(argv[cursor]);
	(not_env(argv[cursor])) ? add_var_to_export(argv[cursor])
		: remove_from_export(argv[cursor]);
	return ;
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
	int			error;

	(void)envp;
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
				if_valid_name(&error, argv, cursor);
		}
	}
	else
		export();
	return ((error == 0) ? 0 : 1);
}
