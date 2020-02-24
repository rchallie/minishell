/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/19 12:46:42 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/20 10:24:47 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incs/minishell.h"

int main(int ac, char **av)
{
	char	*entry;
	char	*treated;
	int		ret;

	(void)ac;
	(void)av;
	ret = 1;
	entry = NULL;
	treated = NULL;
	default_term_mode();
	while (ret == SUCCESS)
	{
		char *pwd;
		if (!get_pwd(&pwd))
			return (ERROR);
		printf("PWD : %s\n", pwd);
		write(0, "minishell % > ", 14);
		ret = line_edition(&entry);
		printf("\nentry = |%s|\n", entry);
		treated = NULL;
		if (!sanitize(entry, &treated))
			return (0);
	}
	exit(0);
}