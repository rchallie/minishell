/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/06/19 12:50:35 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/21 13:04:23 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *str, char c)
{
	size_t		i;

	i = 0;
	while (*str)
	{
		while (*str && *str == c)
			str++;
		if (*str && *str != c)
		{
			i++;
			while (*str && *str != c)
				str++;
		}
	}
	return (i);
}

static size_t	word_length(const char *str, char c)
{
	size_t i;

	i = 0;
	while (str[i] != '\0' && str[i] != c)
	{
		i++;
	}
	return (i);
}

static void		clean(char **arr, size_t size)
{
	size_t i;

	i = 0;
	while (size)
		free(arr[size--]);
	free(arr);
}

char			**ft_split(const char *str, char c)
{
	char		**arr;
	size_t		i;
	size_t		w_len;
	size_t		w_count;

	if (str == NULL ||
	(arr = (char **)malloc(sizeof(char *) * (count_words(str, c) + 1))) == NULL)
		return (NULL);
	w_count = count_words(str, c);
	i = 0;
	while (i < w_count)
	{
		w_len = word_length(str, c);
		if (w_len > 0)
		{
			if (!(arr[i++] = ft_substr(str, 0, w_len)))
			{
				clean(arr, i);
				return (NULL);
			}
		}
		str += w_len + 1;
	}
	arr[i] = NULL;
	return (arr);
}
