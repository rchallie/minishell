/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 15:04:28 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/25 16:05:34 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		overwrite_in_file(t_minishell *ms);

int		treat_output(t_minishell *ms)
{
	// si > < >> continue de treat
	// si | appel cmd et return 1
	// si ; juste return 0
	// check i
	ms->seq_cursor++;
	while (ms->sequence[ms->seq_cursor]
		&& !(ms->sequence[ms->seq_cursor] >= 3
		&& ms->sequence[ms->seq_cursor] <= 7))
	{
		printf("Treat in : %s\n", ms->treated[ms->seq_cursor]);		
		ms->seq_cursor++;
	}
	printf("Treat : %s\n", ms->treated[ms->seq_cursor]);
	if (ms->sequence[ms->seq_cursor] && ms->sequence[ms->seq_cursor] == 3)
	{
		if (overwrite_in_file(ms))
			return (1);
		else
		{
			exit(120);
		}
	}
	return (5);
}

// A mettre dans spec
int		overwrite_in_file(t_minishell *ms)
{
	(void)ms;
	// ms->seq_cursor++;
	// printf("Over : %s\n", ms->treated[ms->seq_cursor]);
	// if(!ms->sequence[ms->seq_cursor] || is_spec(ms->treated[ms->seq_cursor]))
	// {
	// 	printf("Error\n");
	// 	exit(0);
	// }
	// int fd;
	// if (!(fd = open(ms->treated[ms->seq_cursor], O_CREAT | O_RDWR)))
	// {	//get l'erreur donner dans errno
	// 	printf("Error overwrite : FD : %d\n", fd)	;
	// 	exit(1);
	// }
	// char c = '\0';
	// printf("fd : %d\n", fd);
	// write(fd, &c, 1);
	// try si on peut open le file
	// si oui on le detruit
	// on le creer 
	// on get treated[num + 1] 
	// on check le treat
	// si treat ne return pas 1,2,3,4
	// on le fill avec l'output
	return (SUCCESS);
}