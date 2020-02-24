/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 16:30:57 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/24 09:09:02 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"


static int		add_word_to_tab(char *word, char ***treated)
{
	char	**new_tab;
	int		treated_len = 0;
	int		i = 0;

	if (!treated || !*treated)
	{
		if (!(new_tab = (char **)malloc(sizeof(char *) * 2)))
			return (ERROR);
		ft_bzero(new_tab, sizeof(char *) * 2);
		*new_tab = ft_strdup(word);
		*treated = new_tab;
		return (SUCCESS);
	}
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

int				sanitize(char *entry, char ***treated)
{
	char	*word;
	int		up;
	
	printf("\nbef san entry = |%s|\n", entry);
	write(1, "aa\n", 3);
	if (!entry || !*entry || entry[0] == '\n')
	{
		if (!(word = (char *)malloc(sizeof(char) * 1)))
			return (ERROR);
		word[0] = '\0';
		add_word_to_tab(word, treated);
		return (SUCCESS);
	}
	up = 0;
	while (*(entry + up))
	{
		while (ft_is_whitespace(*(entry + up)))
			up++;
		word = NULL;
		up += get_word((entry + up), &word);
		add_word_to_tab(word, treated);
		while (ft_is_whitespace(*(entry + up)))
			up++;
		if(!(up = check_special_chars(treated, entry, up)))
			return (ERROR);
		while (ft_is_whitespace(*(entry + up)))
			up++;
		free(word);
	}
	write(1, "bb\n", 3);
	free(entry);
	return (SUCCESS);
}
