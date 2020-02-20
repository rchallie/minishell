/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 11:06:27 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/10 14:19:16 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *dest, int val, size_t len)
{
	unsigned char *ptr;

	ptr = (unsigned char *)dest;
	while (len-- > 0)
	{
		*ptr++ = val;
	}
	return (dest);
}
