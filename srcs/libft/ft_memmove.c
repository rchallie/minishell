/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 11:18:02 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/10 13:45:51 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t len)
{
	const char	*s;
	char		*d;
	char		*lasts;
	char		*lastd;

	s = src;
	d = dest;
	if (dest == NULL & src == NULL)
		return (NULL);
	if (d < s)
		while (len--)
			*d++ = *s++;
	else
	{
		lasts = (char *)s + (len - 1);
		lastd = (char *)d + (len - 1);
		while (len--)
			*lastd-- = *lasts--;
	}
	return (dest);
}
