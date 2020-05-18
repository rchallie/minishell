/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:27:25 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/18 18:44:16 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int			ft_is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\v' || c == '\n' || c == '\r');
}

int			ft_secure_strlen(const char *str)
{
	int		len;

	len = 0;
	if (!str)
		return (0);
	while (str[len])
		len++;
	return (len);
}

/*
** Function : add_char_to_word
** -------------------------
**		Add a char to the end of the string.
**
**		(char *)word	: the string.
**		(char)	c		: the char.
**
**		return : A new string with the char in end of it.
*/

char		*add_char_to_word(char *word, char c)
{
	char	*save_word;
	int		new_word_len;

	if (!word)
	{
		if (!(word = (char *)ft_memalloc(sizeof(char) * 2)))
			return (ERROR);
		word[0] = c;
		word[1] = '\0';
		return (word);
	}
	new_word_len = ft_secure_strlen(word) + 2;
	save_word = word;
	if (!(word = (char *)ft_memalloc(sizeof(char) * new_word_len)))
		return (ERROR);
	ft_memcpy(word, save_word, ft_secure_strlen(save_word));
	word[ft_secure_strlen(save_word)] = c;
	return (word);
}

/*
** Function : add_char_to_word
** -------------------------
**		Add a char to the end of the string.
**
**		(char *)word	: the string.
**		(char)	c		: the char.
**
**		return : A new string with the char in end of it.
*/

char		*add_char_to_word_free(char *word, char c)
{
	char	*save_word;
	int		new_word_len;

	if (!word)
	{
		if (!(word = (char *)ft_memalloc(sizeof(char) * 2)))
			return (ERROR);
		word[0] = c;
		word[1] = '\0';
		return (word);
	}
	new_word_len = ft_secure_strlen(word) + 2;
	save_word = word;
	if (!(word = (char *)ft_memalloc(sizeof(char) * new_word_len)))
		return (ERROR);
	ft_memcpy(word, save_word, ft_secure_strlen(save_word));
	word[ft_secure_strlen(save_word)] = c;
	free(save_word);
	return (word);
}
