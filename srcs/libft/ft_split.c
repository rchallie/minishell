/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/29 17:26:55 by excalibur         #+#    #+#             */
/*   Updated: 2020/08/09 18:14:44 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>
static int		ft_hm(char const *s, char c)
{
	size_t	nbr;
	int		i;

	nbr = 0;
	i = 0;
	while (s[i])
	{
		while (s[i] == c)
			i++;
		if (i > 0 && s[i] && s[i - 1] == c)
			nbr++;
		if (s[i])
			i++;
	}
	if (nbr == 0 && s[i - 1] == c)
		return (0);
	if (s[0] != c)
		nbr++;
	return (nbr);
}

static char		**ft_mal(char **strs, char const *s, char c)
{
	size_t	count;
	int		i;
	int		h;

	count = 0;
	i = 0;
	h = 0;
	//write(1, "debut mal\n", 10);
	while (s[h])
	{
	//	write(1, "while mal : ", 12);
	//	write(1, &s[h], 1);
	//	write(1, "\n", 1);
		if (s[h] != c)
		{
			count++;
	//		write(1, "in first if\n", 12);
		}
		else if (h > 0 && s[h - 1] != c)
		{
	//		write(1, "MEH\n", 4);
	//		printf("Pointer strs[i] = %p\n", &strs[i]);
			strs[i] = malloc( (count + 1) * sizeof *strs[i]);
	//		write(1, "MEH2\n", 5);
			if (!strs[i])
				return (0);
	//		write(1, "MEH3\n", 5);
			count = 0;
	//		write(1, "MEH4\n", 5);
			i++;
		}
	//	write(1, "PLOP\n", 5);
		if (s[h + 1] == '\0' && s[h] != c)
			if (!(strs[i] = malloc(sizeof(char) * count + 1)))
				return (0);	
	//	write(1, "PLOP2\n", 6);
		h++;
	//	write(1, &s[h], 1);
	//	write(1, "\n", 1);
	}
	//write(1, "mal\n", 4);
	return (strs);
}

static char		**ft_cpy(char **strs, char const *s, char c)
{
	int i;
	int j;
	int h;

	i = 0;
	j = 0;
	h = 0;
	while (s[h])
	{
		if (s[h] != c)
			strs[i][j++] = s[h];
		else if (h > 0 && s[h - 1] != c)
			if (h != 0)
			{
				strs[i][j] = '\0';
				j = 0;
				i++;
			}
		if (s[h + 1] == '\0' && s[h] != c)
			strs[i][j] = '\0';
		h++;
	}
	// write(1, "cpy\n", 2);
	return (strs);
}

char			**ft_split(char const *s, char c)
{
	char	**rtn = NULL;
	int		nbr_w;

	// write(1, "a\n", 2);
	if (!s || !*s)
	{
		if (!(rtn = malloc(sizeof(char *) * 1)))
			return (NULL);
		*rtn = (void *)0;
		return (rtn);
	}
	// write(1, "b\n", 2);
	nbr_w = ft_hm(s, c);
	// printf("NBR WORD = %d\n", nbr_w);
	// write(1, "c\n", 2);
	rtn = malloc(sizeof(char *) * (nbr_w + 1));
	// write(1, "d\n", 2);
	if (!rtn)
		return (0);
	ft_bzero(rtn, sizeof(char *) * (nbr_w + 1));
	// write(1, "e\n", 2);
	if (ft_mal(rtn, s, c) != 0)
		ft_cpy(rtn, s, c);
	else
	{
		free(rtn);
		return (NULL);
	}
	// write(1, "AUREVOIR\n", 9);
	rtn[nbr_w] = (void *)0;
	return (rtn);
}