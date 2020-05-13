/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 10:23:19 by rchallie          #+#    #+#             */
/*   Updated: 2020/05/13 16:24:50 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include "../../incs/minishell.h"

/*
** Function: echo_
** ------------
** 		Write arguments to the standard output.
**		Display the ARGs on the standard output followed by a newline.
**
**		(int)		argc : argument count.
**		(char **)	argv : arguments.
**		(char **)	envp : environment variables.
**
**		returns: return 1
*/

int				echo_(
	int argc,
	char **argv,
	char **envp
)
{
	int		cursor;
	int		has_opt;

	(void)argc;
	(void)envp;
	cursor = 1;
	has_opt = 0;
	if (argv[cursor])
		has_opt = (!ft_strcmp(argv[cursor], "-n")) ? 1 : 0;
	cursor += has_opt;
	while (argv[cursor])
	{
		write(1, argv[cursor], ft_secure_strlen(argv[cursor]));
		if (argv[cursor + 1] != NULL)
			write(1, " ", 1);
		cursor++;
	}
	if (!has_opt)
		write(1, "\n", 1);
	return (0);
}
