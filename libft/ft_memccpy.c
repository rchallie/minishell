/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 11:15:21 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/08 11:50:56 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dest, const void *src, int c, size_t n)
{
	const char	*sourc;
	char		*dst;

	sourc = (const char *)src;
	dst = (char *)dest;
	while (n--)
	{
		*dst++ = *sourc;
		if (*sourc++ == (char)c)
		{
			return ((void *)dst);
		}
	}
	return (NULL);
}
