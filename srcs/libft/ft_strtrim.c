/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 14:33:26 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/21 13:03:06 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_trimmed(const char c, const char *set)
{
	if (set != NULL)
	{
		while (*set != '\0')
		{
			if (c == *set++)
				return (1);
		}
	}
	return (0);
}

char		*ft_strtrim(char const *s1, char const *set)
{
	const char	*end;

	if (s1 == NULL)
		return (NULL);
	end = s1 + ft_strlen(s1) - 1;
	while (*s1 != '\0' && is_trimmed(*s1, set))
		s1++;
	while (end >= s1 && is_trimmed(*end, set))
		end--;
	return (ft_substr(s1, 0, end + 1 - s1));
}
