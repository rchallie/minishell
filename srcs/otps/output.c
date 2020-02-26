/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 15:04:28 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/26 13:40:39 by rchallie         ###   ########.fr       */
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
	// printf("Cursor 2 : %d\n", ms->seq_cursor);

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
	ms->seq_cursor++;
	if(!ms->sequence[ms->seq_cursor]
		|| is_char_spec(ms->treated[ms->seq_cursor]))
	{
		printf("Error\n");
		exit(0);
	}
	int fd;
	if (!(fd = open(ms->treated[ms->seq_cursor],
		O_CREAT | O_RDWR | O_TRUNC , S_IRWXG | S_IRWXU | S_IRWXO)))
	{	//get l'erreur donner dans errno
		printf("Error overwrite : FD : %d\n", fd)	;
		exit(1);
	}
	if (treat_output(ms) == 5)
		write(fd, ms->output, ft_secure_strlen(ms->output));
	close(fd);
	return (SUCCESS);
}