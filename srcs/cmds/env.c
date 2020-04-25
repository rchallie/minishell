/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:48:15 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/22 16:21:43 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

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
