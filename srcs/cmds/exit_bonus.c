/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 17:24:07 by rchallie          #+#    #+#             */
/*   Updated: 2020/10/01 19:48:01 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

/*
**	Function: array to u_int8_t
**	--------------------
*/

static int		is_space(char c)
{
	if (c == ' ' || c == '\t' || c == '\r' || c == '\v'
			|| c == '\n' || c == '\f')
		return (1);
	return (0);
}

/*
**	Function: if_numeric_str
**	--------------------
**		Check if string is numeric (from machine_minus to machine_max)
**
**		(char *)	the string
*/

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
**	Function: if_numeric_str
**	--------------------
**		Check if string is numeric (from machine_minus to machine_max)
**
**		(char *)	the string
*/

int				is_numeric_str(char *str)
{
	int			i;
	int			sign;
	int			nb;

	nb = 0;
	i = 0;
	sign = 0;
	sign = (str[i] == '+' || str[i] == '-') ? ++i : i;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	if (ft_secure_strlen(str) == (ft_secure_strlen("9223372036854775808")
		+ sign))
	{
		nb = 10 * (str[sign + 17] - '0') + (str[sign + 18] - '0');
		if (str[0] == '-' && nb > 8)
			return (0);
		else if (str[0] != '-' && nb > 7)
			return (0);
	}
	return (1);
}

/*
**	Function: if_not_numeric_str_exit
**	--------------------
**		Check if string is numeric.
**		If not prints the right error msg.
** 		Exit command. Leave minishell propely.
**
**		(char **)	argv : arguments.
*/

static void		if_not_numeric_str_exit(char **argv)
{
	if (!is_numeric_str(argv[1]))
	{
		(isatty(0)) ? ft_printf(2, "exit\n") : 0;
		ft_printf(2, "minishell: exit: %s: numeric argument required\n",
			argv[1]);
		exit(2);
	}
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
		if_not_numeric_str_exit(argv);
		(isatty(0)) ? ft_printf(2, "exit\n") : 0;
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return ((is_numeric_str(argv[1]) ? 1 : 2));
	}
	free_double_char_tab(g_ms.treated);
	free(g_ms.sequence);
	(g_envp) ? free_double_char_tab(g_envp) : 0;
	(g_export_vars) ? free_double_char_tab(g_export_vars) : 0;
	(g_pwd) ? free(g_pwd) : 0;
	if (argc == 2)
	{
		if_not_numeric_str_exit(argv);
		(isatty(0)) ? ft_printf(2, "exit\n") : 0;
		exit(ft_atou(argv[1]));
	}
	(argc == 1 && isatty(0)) ? ft_printf(2, "exit\n") : 0;
	exit(g_ms.last_cmd_rtn);
}
