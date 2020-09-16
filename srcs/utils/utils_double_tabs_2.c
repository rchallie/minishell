/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_double_tabs_2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/13 16:44:03 by excalibur         #+#    #+#             */
/*   Updated: 2020/05/26 17:12:36 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int			get_double_char_tab_len(char **tabl)
{
	int		i;

	i = 0;
	if (!tabl || !*tabl)
		return (0);
	while (tabl[i])
		i++;
	return (i);
}

static char	**new_double_char_tab(size_t double_tab_size)
{
	char	**new_tab;

	if (!(new_tab = (char **)malloc(double_tab_size)))
		return (NULL);
	ft_bzero(new_tab, double_tab_size);
	return (new_tab);
}

char	**new_double_char_tab_init(size_t double_tab_size, char *str_init)
{
	char	**new_tab;

	if ((new_tab = new_double_char_tab(double_tab_size)) == NULL)
		return (NULL);
	*new_tab = ft_strdup(str_init);
	return (new_tab);
}

static int	strdup_tab(char *word, char ***new_tab, char ***treated)
{
	int		i;
	int		treated_len;
	char	**save_treated;

	i = -1;
	save_treated = *treated;
	treated_len = get_double_char_tab_len(*treated);
	if ((*new_tab = new_double_char_tab(sizeof(char *) * (treated_len + 2)))
		== NULL)
		return (ERROR);
	while (++i < treated_len)
		(*new_tab)[i] = ft_strdup(treated[0][i]);
	(*new_tab)[i] = ft_strdup(word);
	*treated = *new_tab;
	free_double_char_tab(save_treated);
	return (SUCCESS);
}

int			add_word_to_tab(char *word, char ***treated)
{
	char	**new_tab;

	if (!word)
		return (SUCCESS);
	if (!treated || !*treated)
	{
		if ((*treated = new_double_char_tab_init(sizeof(char *) * 2, word))
			== NULL)
			return (ERROR);
		return (SUCCESS);
	}
	return (strdup_tab(word, &new_tab, treated));
}
