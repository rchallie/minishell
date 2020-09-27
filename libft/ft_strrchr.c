/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/08 11:50:15 by thervieu          #+#    #+#             */
/*   Updated: 2019/10/08 11:54:02 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*rtn_val;

	rtn_val = 0;
	while (*s)
	{
		if (*s == c)
			rtn_val = (char *)s;
		s++;
	}
	if (*s == c)
		return ((char *)s);
	return (rtn_val);
}
