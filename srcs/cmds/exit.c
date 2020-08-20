/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: excalibur <excalibur@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 17:24:07 by rchallie          #+#    #+#             */
/*   Updated: 2020/08/11 22:11:22 by excalibur        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"


/*
**	Function: array to u_int8_t
**	--------------------
*/


static int			is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\v'
			|| c == '\n' || c == '\f')
		return (1);
	return (0);
}

static u_int8_t	ft_atou(const char *str)
{
	int			i;
	int			sign;
	u_int64_t	nb;

	i = 0;
	if (str == NULL && *str == '0')
		return (0);
	nb = 0;
	sign = 0;
	if (str[i] == '+' || str[i] == '-')
		sign = (str[i++] == '-') ? 1 : 0;
	while (is_space(str[i]))
		i++;
	while (ft_isdigit(str[i]))
		nb = (nb * 10) + (str[i++] - '0');
	if (sign)
		return (-nb);
	return (nb);
}


/*
**	Function: exit_minishell
**	--------------------
**		Exit command. Leave minishell propely.
**
**		(char *)	path : final path to directory.
**		(int)		curosr : arguments cursor.
**		(char **)	argv : arguments.
*/

int				exit_minishell(
	int argc,
	char **argv,
	char **env
)
{
	(void)argc;
	(void)argv;
	(void)env;
	default_term_mode();
	if (argc > 2)
	{
		ft_putstr("exit\nminishell: exit: too many arguments\n");
		return (1);
	}
	free_double_char_tab(g_ms.treated);
	free(g_ms.sequence);
	if (g_envp)
		free_double_char_tab(g_envp);
	if (g_export_vars)
		free_double_char_tab(g_export_vars);
	ft_putstr("exit\n");
	if (argc == 2)
		exit(ft_atou(argv[1]));
	exit(g_ms.last_cmd_rtn);
}
