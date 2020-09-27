/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 15:19:46 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/10 15:19:47 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_strlen2(const char *src)
{
	size_t i;

	i = 0;
	while (src[i])
		i++;
	return (i);
}

static void		*ft_memcpy2(void *dest, const void *src, size_t len)
{
	char		*d;
	const char	*s;

	d = dest;
	s = src;
	if (dest == NULL && src == NULL)
		return (NULL);
	while (len--)
		*d++ = *s++;
	return (dest);
}

size_t			ft_strlcpy(char *dst, const char *src, size_t maxlen)
{
	size_t srclen;

	if (dst == NULL && src == NULL)
		return (0);
	srclen = ft_strlen2(src);
	if (srclen + 1 < maxlen)
		ft_memcpy2(dst, src, srclen + 1);
	else if (maxlen != 0)
	{
		ft_memcpy2(dst, src, maxlen - 1);
		dst[maxlen - 1] = '\0';
	}
	return (srclen);
}
