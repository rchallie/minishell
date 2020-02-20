/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 15:44:53 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/20 18:08:26 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static int check_arg_norme(void *content)
{
	char *matter;
	char *contentu;
	int i;

	i = 0;
	matter = NULL;
	contentu = ft_strdup((char *)content);
	get_word(contentu, &matter);
	// printf("contentu : |%s|\n", contentu);
	
	printf("matter : |%s|\n", matter);
	while (matter[i])
	{
		if (!ft_isalnum(matter[i]) && matter[i] != '=')
		{
			printf("char %d\n", matter[i]);
			return (error_identifier("export", (const char *)matter,
				"not a valid identifier"));
		}
		i++;
	}
	return (SUCCESS);
}

int export(t_args *args)
{
	// write(1, "a\n", 2);
	while (args)
	{
		// write(1, "b\n", 2);
		// printf("args : %p | content : %p | next : %p \n", &args, &args->content, &args->next);
		if (!check_arg_norme(args->content)) //Content need to be malloced
			return (ERROR);
		// write(1, "c\n", 2);
		args = args->next;
	}
	return (SUCCESS);
}