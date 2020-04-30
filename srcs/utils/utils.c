/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:27:25 by rchallie          #+#    #+#             */
/*   Updated: 2020/04/30 15:27:45 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
** Function : add_char_to_word_front
** -------------------------
**		Add a char to the front of a string.
**
**		(char *)word	: the string.
**		(char)	c		: the char.
**
**		return : A new string with the char in front of it.
*/

char			*add_char_to_word_front(char *word, char c)
{
	int		i;
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
	word[0] = c;
	i = 1;
	ft_memcpy(word + i, save_word + (i - 1),
		ft_secure_strlen(save_word + (i - 1)));
	return (word);
}

/*
** Function : add_char_to_word_ads
** -------------------------
**	(voir avec thervieu).
*/

char			*add_char_to_word_ads(char *word, char c, int nb)
{
	int		i;
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
	i = -1;
	new_word_len = ft_secure_strlen(word) + 2;
	save_word = word;
	if (!(word = (char *)ft_memalloc(sizeof(char) * new_word_len)))
		return (ERROR);
	while (++i <= nb)
		word[i] = save_word[i];
	word[i] = c;
	i++;
	ft_memcpy(word + i, save_word + (i - 1),
		ft_secure_strlen(save_word + (i - 1)));
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

char			*add_char_to_word(char *word, char c)
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
** Not sure about that.
*/

int				insert_in_string(char **str, char *to_insert, int start)
{
	int		str_len;
	char	*new_str;

	str_len = ft_secure_strlen(*str);
	if (start > str_len)
		start = str_len;
	new_str = ft_substr(*str, 0, start);
	new_str = ft_strjoin(new_str, to_insert);
	new_str = ft_strjoin(new_str, *(str + start));
	*str = new_str;
	return (SUCCESS);
}
