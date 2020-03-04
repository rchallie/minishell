/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 16:30:57 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/04 15:48:16 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		add_word_to_tab(char *word, char ***treated)
{
	char	**new_tab;
	char	**save_treated;
	int		treated_len = 0;
	int		i = 0;

	if (!word)
		return (SUCCESS);
	if (!treated || !*treated)
	{
		if (!(new_tab = (char **)malloc(sizeof(char *) * 2)))
			return (ERROR);
		ft_bzero(new_tab, sizeof(char *) * 2);
		*new_tab = ft_strdup(word);
		*treated = new_tab;
		return (SUCCESS);
	}
	save_treated = *treated;
	treated_len = get_double_char_tab_len(*treated);
	i = 0;
	if (!(new_tab = (char **)malloc(sizeof(char *) * (treated_len + 2))))
		return (ERROR);
	ft_bzero(new_tab, sizeof(char *) * (treated_len + 2));
	while (i < treated_len)
	{
		new_tab[i] = ft_strdup(treated[0][i]);
		i++;
	}
	new_tab[i] = ft_strdup(word);

	*treated = new_tab;
	free_double_char_tab(save_treated);
	return (SUCCESS);
}

static int		special_char(char ***treated, char *entry, int up, char c)
{
	char	*word;
	
	word = NULL;
	if (*(entry + up) == c)
	{
		if (word)
			free(word);
		word = NULL;
		while (*(entry + up) == c)
		{
			word = add_char_to_word(word, *(entry + up));
			up++;
		}
		if (ft_secure_strlen(word) > 2)
		{
			ft_printf("minishell: syntax error near unexpected token `%c%c'\n", c, c); // ||| = 1, >>/<</>/< = 2
			return (ERROR);
		}
		else
			add_word_to_tab(word, treated);
		while (ft_is_whitespace(*(entry + up)))
		up++;
	}
	return (up);
}

static int		check_special_chars(char ***treated, char *entry, int up)
{
	if (!(up = special_char(treated, entry, up, '>'))
		|| !(up = special_char(treated, entry, up, '<'))
		|| !(up = special_char(treated, entry, up, '|'))
		|| !(up = special_char(treated, entry, up, '>'))
		|| !(up = special_char(treated, entry, up, ';')))
			return (ERROR);
	return (up);
}

int				sanitize(t_minishell *ms, char *entry, char ***treated)
{
	char	*word;
	int		up;
	
	// printf("\nbef san entry = |%s|\n", entry);
	if (!entry || !*entry || entry[0] == '\n')
	{
		if (!(word = (char *)malloc(sizeof(char) * 1)))
			return (ERROR);
		word[0] = '\0';
		add_word_to_tab(word, treated);
		add_word_to_tab(ft_strdup("\n"), treated);
		return (SUCCESS);
	}
	up = 0;
	while (*(entry + up))
	{
		while (ft_is_whitespace(*(entry + up)))
			up++;
		word = NULL;
		up += get_word(ms, (entry + up), &word);
		add_word_to_tab(word, treated);
		while (ft_is_whitespace(*(entry + up)))
			up++;
		if(!(up = check_special_chars(treated, entry, up)))
			return (ERROR);
		while (ft_is_whitespace(*(entry + up)))
			up++;
		free(word);
	}
	add_word_to_tab(ft_strdup("\n"), treated);
	free(entry);
	return (SUCCESS);
}
