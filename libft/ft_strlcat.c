/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/10 14:46:51 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/10 14:46:52 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t			ft_strlcat(char *dst, const char *src, size_t maxlen)
{
	size_t srclen;
	size_t dstlen;

	srclen = ft_strlen(src);
	dstlen = ft_strnlen(dst, maxlen);
	if (maxlen != 0 && dstlen < maxlen)
	{
		while (*dst != '\0')
			++dst;
		while (*src != '\0' && (maxlen - dstlen - 1) > 0)
		{
			*dst++ = *src++;
			maxlen--;
		}
		*dst = '\0';
	}
	return (dstlen + srclen);
}
