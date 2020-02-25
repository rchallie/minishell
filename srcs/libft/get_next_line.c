/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <thervieu@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/14 12:21:43 by thervieu          #+#    #+#             */
/*   Updated: 2019/11/26 17:37:40 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*append_str(char *str, char buff)
{
	int		len;
	char	*dst;

	len = 0;
	if (!str)
		return (ft_strdup(&buff));
	while (str[len])
		len++;
	if ((dst = malloc(sizeof(char) * (len + 2))) == NULL)
		return (NULL);
	len = 0;
	while (str[len])
	{
		dst[len] = str[len];
		len++;
	}
	dst[len] = buff;
	dst[len + 1] = '\0';
	return (dst);
}

int		get_next_line(int fd, char **line)
{
	static char		*str;
	char			buff;
	char			*to_free;
	int				reading;

	reading = 1;
	if (fd < 0 || !line)
		return (-1);
	buff = '\0';
	while (reading != 0 && buff != '\n')
	{
		if ((reading = read(fd, &buff, 1)) == -1)
			return (-1);
		to_free = str;
		str = append_str(str, buff);
		free(to_free);
	}
	*line = str;
	if (reading == 0)
		return (0);
	return (1);
}
