/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:27:25 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/20 19:01:04 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

char		*add_char_to_word(char *word, char c)
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
		// printf("WORD : |%s|\n", word);
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
	// printf("WORD : %s\n", word);
	// if (save_word)
	// 	free(save_word);
	return (word);
}

int		get_word(char *entry, char **word)
{
	int simple_q;
	int double_q;
	int char_count;

	simple_q = 0;
	double_q = 0;
	char_count = 0;
	// printf("word : |%s|\n", *word);
	// if (*word)
		// printf("PLPO\n");
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
	{
		// printf("s_q : %d | d_q : %d\n", simple_q, double_q);
		exit(1);
	}
	return (char_count);
}