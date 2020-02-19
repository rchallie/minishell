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

char	*append_line(char *str)
{
	char	*dst;
	int		i;

	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if ((dst = malloc(sizeof(char) * (i + 1))) == NULL)
		return (NULL);
	i = 0;
	while (str[i] != '\n' && str[i] != '\0')
	{
		dst[i] = str[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*redefine_str(char *str)
{
	int		i;
	int		j;
	char	*dst;

	i = 0;
	if (str == NULL)
		return (NULL);
	while (str[i] != '\n' && str[i] != '\0')
		i++;
	if (str[i] == '\0')
	{
		free(str);
		return (NULL);
	}
	if ((dst = malloc(sizeof(char) * ((ft_strlen(str) - i) + 1))) == NULL)
		return (NULL);
	j = 0;
	i++;
	while (str[i] != '\0')
		dst[j++] = str[i++];
	dst[j] = '\0';
	free(str);
	return (dst);
}

int		has_backslash_n(char *str)
{
	int		i;

	i = 0;
	if (!str)
		return (0);
	while (str[i])
	{
		if (str[i] == '\n')
			return (1);
		i++;
	}
	return (0);
}

int		get_next_line(char **line)
{
	static char		*str;
	char			*buff;
	char			*to_free;
	int				reading;

	reading = 1;
	if (!line ||
		(buff = malloc(sizeof(char) * 1)) == NULL)
		return (-1);
	while (has_backslash_n(str) == 0 && reading != 0)
	{
		if ((reading = read(0, buff, 1)) == -1)
			return (-1);
		buff[reading] = '\0';
		to_free = str;
		str = ft_strjoin(str, buff);
		free(to_free);
	}
	free(buff);
	*line = append_line(str);
	str = redefine_str(str);
	if (reading == 0)
		return (0);
	return (1);
}
