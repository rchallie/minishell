/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sanitize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 16:30:57 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/19 16:57:01 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"
#include <stdio.h>

static char		*add_char_to_word(char *word, char c)
{
	int		new_word_len;
	int		i;
	char	*save_word;

	if (!word)
	{
		if (!(word = (char *)malloc(sizeof(char) * 2)))
			return (ERROR);
		word[0] = c;
		word[1] = '\0';
		return (word);
	}
	i = 0;
	new_word_len = ft_secure_strlen(word) + 2;
	save_word = word;
	if (!(word = (char *)malloc(sizeof(char) * new_word_len)))
		return (ERROR);
	ft_bzero(word, new_word_len);
	while (save_word[i])
	{
		word[i] = save_word[i];
		i++;
	}
	word[i++] = c;
	word[i] = '\0';
	free(save_word);
	return (word);
}

static int		get_word(char *entry, char **word)
{
	int simple_q;
	int double_q;
	int char_count;

	simple_q = 0;
	double_q = 0;
	char_count = 0;
	while (*entry)
	{
		if (*entry == ' ' && !(simple_q || double_q))
			break ;
		if (*entry == '\'' && simple_q == 0 && double_q == 0)
			simple_q = 1;
		else if (*entry == '\'' && double_q == 1)
			*word = add_char_to_word(*word, *entry);
		else if (*entry == '\'')
			simple_q = 0;
		if (*entry == '\"' && double_q == 0 && simple_q == 0)
			double_q = 1;
		else if (*entry == '\"' && simple_q == 1)
			*word = add_char_to_word(*word, *entry);
		else if (*entry == '\"')
			double_q = 0;
		if (*entry != '\'' && *entry != '\"')
			*word = add_char_to_word(*word, *entry);
		if (!word)
			return (ERROR);
		entry++;
		char_count++;
	}
	if (simple_q || double_q)
		exit(1);
	return (char_count);
}

int				sanitize(char *entry, char **treated)
{
	char	*to_free;
	char	*word;
	int		up;

	if (!entry || !*entry)
	{
		if (!(*treated = (char *)malloc(sizeof(char) * 1)))
			return (ERROR);
		*treated[0] = '\0';
		return (SUCCESS);
	}
	up = 0;
	while (*(entry + up))
	{
		word = NULL;
		up += get_word((entry + up), &word);
		while (ft_is_whitespace(*(entry + up))) //ça peut etre un '\0'?
			up++;
		printf("WORD |%s|\n", word); //Join treated et word
		to_free = *treated;
		if (treated[0] != '\0')
		{
			*treated = ft_strjoin(*treated, " ");
			free(to_free);
			to_free = *treated;
		}
		printf("treated before join = |%s|\n", *treated);
		*treated = ft_strjoin(*treated, word);
		printf("treated after join = |%s|\n", *treated);
		free(to_free);
		free(word);
	}
	return (SUCCESS);
}