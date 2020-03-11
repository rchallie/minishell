/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 14:48:15 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/27 09:25:43 by rchallie         ###   ########.fr       */
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

int		env(t_minishell *ms)
{
	char	*env_list;
	int		cursor;
	int		i;
	
	i = 0;
	cursor = ms->seq_cursor;
	env_list = NULL;
	while (ms->envp[i])
	{
		env_list = ft_strjoin(env_list, ms->envp[i]);
		env_list = add_char_to_word(env_list, '\n');
		i++;
	}
	ms->output = env_list;
	if(!ms->has_spec_uf)
		ft_printf("Envlist : \n%s", env_list);
	else
		return (TREAT);
	while (ms->sequence[++cursor] == 2)
		if (check_word(ms->treated[cursor]) >= 1)
			ft_printf("%s\n", ms->treated[cursor]);
	return (SUCCESS);
}
