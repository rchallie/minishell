/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/19 16:56:09 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"
#include <stdio.h>

int main()
{
	printf("test : %zu\n", ft_strlen("test"));
	char *entry = "\"ec\'h \'\"o\"\" test";
	char *treated_entry = NULL;
	if (!sanitize(entry, &treated_entry))
		return (0);
	printf("Treated entry : %s\n", treated_entry);
}