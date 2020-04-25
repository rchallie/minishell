/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:48:15 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/25 15:25:33 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

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

int				add_var_to_env(char *var)
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

int		check_word(char *word)
{
	int		i;

	i = 0;
	while (word[i] != '=')
		i++;
	if (i == 0 || word[i] != '=')
		return (ERROR);
	return (SUCCESS);
}

int		env(int argc, char **argv, char **envp)
{
	char	*env_list;
	int		cursor;
	int		i;
	
	(void)argc;
	i = 0;
	cursor = 0;
	env_list = NULL;
	while (envp[i])
	{
		env_list = ft_strjoin(env_list, envp[i]);
		env_list = add_char_to_word(env_list, '\n');
		i++;
	}
	ft_printf("%s", env_list);
	while (argv[++cursor])
		if (check_word(argv[cursor]) >= 1)
			ft_printf("%s\n", argv[cursor]);
	return (SUCCESS);
}
