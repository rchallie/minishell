/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 16:27:25 by rchallie          #+#    #+#             */
/*   Updated: 2020/09/27 20:35:17 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void		quote_error(char **startword, char **entry_addr,
char **save_startword, int simple_q)
{
	char		*test;
	int			startword_advencement;

	(simple_q == 1) ? ft_printf(STDOUT_FILENO, "squote > ")
		: ft_printf(STDOUT_FILENO, "dquote > ");
	test = edit_line();
	startword_advencement = *startword - *save_startword;
	test = ft_strjoin("\n", test);
	*startword = ft_strjoin(*save_startword, test);
	*entry_addr = ft_strjoin(*entry_addr, test);
	*save_startword = *startword;
	*startword += startword_advencement;
}

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
