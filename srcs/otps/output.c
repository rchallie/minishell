/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   output.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 15:04:28 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/04 14:55:33 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int		overwrite_in_file(t_minishell *ms);
int		add_in_file(t_minishell *ms);


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
	// printf("Treat : %s\n", ms->treated[ms->seq_cursor]);
	if (ms->sequence[ms->seq_cursor] && ms->sequence[ms->seq_cursor] == 3)
	{
		if (overwrite_in_file(ms))
			return (1);
	}
	else if (ms->sequence[ms->seq_cursor] && ms->sequence[ms->seq_cursor] == 4)
	{
		if (add_in_file(ms))
			return (2);
	}
	return (5);
}

// A mettre dans spec
int		add_in_file(t_minishell *ms)
{
	int		fd;
	char	buf[2];
	char	*content;

	buf[0] = 0;
	buf[1] = 0;
	content = NULL;
	ms->seq_cursor++;	
	if(!ms->sequence[ms->seq_cursor]
		|| is_char_spec(ms->treated[ms->seq_cursor]))
	{
		printf("Error (2) : SEQ : %d | TREATED : %s\n", ms->sequence[ms->seq_cursor], ms->treated[ms->seq_cursor]);
		exit(0);
	}
	if (!(fd = open(ms->treated[ms->seq_cursor],
		O_CREAT | O_RDWR , S_IRWXG | S_IRWXU | S_IRWXO)))
	{
		printf("Error add_in_file : FD : %d\n", fd)	;
		exit(1);
	}
	while (read(fd, buf, 1));
	if (treat_output(ms) == 5)	
		write(fd, ms->output, ft_secure_strlen(ms->output));
	return (SUCCESS);
}

// A mettre dans spec
int		overwrite_in_file(t_minishell *ms)
{
	ms->seq_cursor++;
	if(!ms->sequence[ms->seq_cursor]
		|| is_char_spec(ms->treated[ms->seq_cursor]))
	{
		printf("Error (1) : SEQ : %d | TREATED : %s\n", ms->sequence[ms->seq_cursor], ms->treated[ms->seq_cursor]);
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