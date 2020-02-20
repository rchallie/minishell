/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/20 08:53:36 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"
#include <stdio.h>

int main(int ac, char **av)
{
	char	*entry;
	char	*treated;
	
	(void)ac;
	(void)av;
	entry = NULL;
	treated = NULL;
	while (get_next_line(&entry) == SUCCESS)
	{
		printf("test : %zu\n", ft_strlen("test"));
		printf("entry : %s\n", entry);
		treated = NULL;
		if (!sanitize(entry, &treated))
			return (0);
		printf("Treated entry : |%s|\n", treated);
	}
	exit(0);
}