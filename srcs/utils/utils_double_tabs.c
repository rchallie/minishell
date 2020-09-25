/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_double_tabs.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/04/21 16:14:35 by excalibur         #+#    #+#             */
/*   Updated: 2020/09/26 00:04:04 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int			double_char_tab_remove(char **to_remove, char ***array)
{
	int		i;
	int		has_found_to_remove;
	char	**new_array;

	i = 0;
	has_found_to_remove = 0;
	if (array == NULL || *array == NULL
		|| to_remove == NULL || *to_remove == NULL)
		return (ERROR);
	if (!(new_array = malloc(sizeof(char *) * get_double_char_tab_len(*array))))
		return (ERROR);
	ft_bzero(new_array, sizeof(char *) * get_double_char_tab_len(*array));
	while ((*array)[i])
	{
		if ((*array)[i] != *to_remove)
			new_array[i - has_found_to_remove] = ft_strdup((*array)[i]);
		else
			has_found_to_remove = 1;
		i++;
	}
	free_double_char_tab((*array));
	*array = new_array;
	return (SUCCESS);
}

int			double_char_tab_contain(char *name, char **from)
{
	if (name == NULL || from == NULL)
		return (ERROR);
	while (*from)
	{
		if (!ft_strncmp(*from, name, ft_secure_strlen(name))
			&& (*from)[ft_secure_strlen(name)] == '\0')
			return (SUCCESS);
		from++;
	}
	return (ERROR);
}

void		free_double_char_tab(char **tab_to_free)
{
	int		i;
	int		tab_len;

	if (!tab_to_free)
		return ;
	tab_len = get_double_char_tab_len(tab_to_free);
	if (tab_len == 0)
	{
		free(tab_to_free);
		return ;
	}
	i = 0;
	while (i < tab_len)
		free(tab_to_free[i++]);
	free(tab_to_free[i]);
	free(tab_to_free);
}

int			dup_double_char_tab(char **src, char ***new_tab)
{
	char	**nt;
	int		i;
	int		tab_len;

	i = 0;
	if (!src)
		return (ERROR);
	tab_len = get_double_char_tab_len(src);
	if (!(nt = malloc(sizeof(char *) * (tab_len + 1))))
		return (ERROR);
	while (i < tab_len)
	{
		nt[i] = ft_strdup(src[i]);
		i++;
	}
	nt[i] = NULL;
	*new_tab = nt;
	return (SUCCESS);
}

char		**double_tab_bubble_sort(char ***sort_me)
{
	char	*tmp;
	char	**save;
	int		i;
	int		j;

	i = -1;
	save = *sort_me;
	while (save[++i])
	{
		j = -1;
		while (save[++j] && save[j + 1])
		{
			if (ft_strcmp((const char *)save[j], (const char *)save[j + 1]) > 0)
			{
				tmp = save[j];
				save[j] = save[j + 1];
				save[j + 1] = tmp;
			}
		}
	}
	return (save);
}
