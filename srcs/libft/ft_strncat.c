/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strncat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/19 11:43:19 by thervieu          #+#    #+#             */
/*   Updated: 2019/11/19 11:47:28 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strncat(char *s1, const char *s2, size_t n)
{
	char	*s;
	size_t	size;

	s = s1;
	s1 += ft_strlen(s1);
	size = ft_strnlen(s2, n);
	s1[size] = '\0';
	ft_memcpy(s1, s2, size);
	return (s);
}
