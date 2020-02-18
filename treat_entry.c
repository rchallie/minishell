/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   treat_entry.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/18 15:03:04 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/18 18:34:11 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SUCCESS 1
#define ERROR 0

static void	ft_bzero(void *s, size_t n)
{
	size_t			i;
	unsigned char	*suc;

	i = 0;
	suc = (unsigned char *)s;
	while (i < n)
	{
		suc[i] = 0;
		i++;
	}
	s = suc;
}

static int secure_strlen(char *s)
{
	int i = 0;
	if (!s || !*s)
		return (0);
	while (s[i])
		i++;
	return (i);
}

static char *add_char_to_word(char *word, char c)
{
	int new_word_len;
	int i;
	char *save_word;
	if (!word)
	{
		if (!(word = (char *)malloc(sizeof(char) * 2)))
			return (ERROR);
		word[0] = c;
		word[1] = '\0';
		return (word);
	}
	i = 0;
	new_word_len = secure_strlen(word) + 2;
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

static int get_word(char *entry, char **word)
{
	/**
	 *  _q = 0 : fermé
	 *	_q = 1 : ouvert
	 * */
	int simple_q;
	int double_q;

	simple_q = 0;
	double_q = 0;
	while (*entry)
	{
		if (*entry == '\'' && simple_q == 0 && double_q == 0)
			simple_q = 1;
		else if (*entry == '\'' &&double_q == 1)
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
		entry++;
	}
	// return nbr ++
	return (SUCCESS);
}

int treat_entry(char *entry, char **treated)
{
	if (!entry || !*entry)
	{
		if (!(*treated = (char *)malloc(sizeof(char) * 1)))
			return (ERROR);
		*treated[0] = '\0';
		return (SUCCESS);
	}
	char *word = NULL;
	get_word(entry, &word);
	printf("WORD |%s|\n", word);
	free(word);
	return (SUCCESS);
}

int main(int argc, char **argv)
{
	(void)argc;
	char *entry = argv[1];
	char *treated_entry;
	if (!treat_entry(entry, &treated_entry))
		return (0);
	printf("Treated entry : %s\n", treated_entry);
	while (1);
}