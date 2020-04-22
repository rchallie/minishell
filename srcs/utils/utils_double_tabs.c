/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_double_tabs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/21 16:14:35 by excalibur         #+#    #+#             */
/*   Updated: 2020/04/21 17:25:01 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

char	*get_env_var_by_name(char *name, char **envp)
{
	int		i;
	char	*rename;
	int		j;

	rename = ft_strdup(name);
	i = 0;
	while (envp[i])
	{
		j = 0;
		while (envp[i][j] && rename[j] && envp[i][j] == rename[j])
			j++;
		if (rename[j] == '\0' && envp[i][j] == '=')
		{
			j++;
			free(rename);
			return (ft_strdup(&envp[i][j]));
		}
		i++;
	}
	(void)envp;
	free(rename);
	return (NULL);
}

void	free_double_char_tab(char **tab_to_free)
{
	int i;
	int tab_len;

	i = 0;
	if (!tab_to_free)
		return ;
	tab_len = get_double_char_tab_len(tab_to_free);
	if (tab_len == 0)
	{
		free(tab_to_free);
		return ;
	}
	while (i < tab_len)
		free(tab_to_free[i++]);
	free(tab_to_free);
}
