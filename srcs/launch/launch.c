/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   launch.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 19:27:44 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/27 19:27:45 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void    starting_ms(char *launching, t_line line, char *str)
{
    int i;
    int j;
    int delay;

	line.start.row = 4;
	line.start.col = 1;
	while (ft_secure_strlen(str) < ft_secure_strlen(launching))
	{
		i = 0;
		str = add_char_to_word(str, launching[ft_secure_strlen(str)]);
		set_curpos(&line);
		ft_putstr_fd(str, 0);
		while (i < 100000)
		{
			j = 0;
			if (ft_secure_strlen(str) < ft_secure_strlen(launching) - 3)
				delay = 20;
			else
				delay = 7000;
			while (j < delay)
				j++;
			i++;
		}
	}

}

void    check_updates(char *launching, t_line line, char *str, int nb)
{
    int i;
    int j;
    int delay;
	line.start.row = (nb == 3) ? 1 : 2;
	line.start.col = 1;
	while (ft_secure_strlen(str) < ft_secure_strlen(launching))
	{
		i = 0;
		str = add_char_to_word(str, launching[ft_secure_strlen(str)]);
		set_curpos(&line);
		ft_putstr_fd(str, 0);
		while (i < 100000)
		{
			j = 0;
			if (ft_secure_strlen(str) < ft_secure_strlen(launching) - nb)
				delay = 20;
			else
				delay = 5000;
			while (j < delay)
				j++;
			i++;
		}
	}

}

void    installing_files_2(t_line line)
{
    int i;
    int j;
    int delay;
    int per = 0;

	line.start.row = 3;
	line.start.col = 19;
    while (per < 101)
    {
        set_curpos(&line);
        ft_printf("%d%%]", per);
        i = 0;
        per % 3 == 0 ? delay = 200: 0;
        per % 5 == 0 ? delay = 300: 0;
        per % 7 == 0 ? delay = 10: 0;
        per % 11 == 0 ? delay = 150: 0;
        per == 99 ? delay = 6000 : 0;
        while (++i < 100000)
		{
			j = 0;
			while (j < delay)
				j++;
		}
        per++;
    }
}

void    installing_files_1(char *launching, t_line line, char *str)
{
    int i;
    int j;
	line.start.row = 3;
	line.start.col = 1;
	while (ft_secure_strlen(str) < ft_secure_strlen(launching))
	{
		i = 0;
		str = add_char_to_word(str, launching[ft_secure_strlen(str)]);
		set_curpos(&line);
		ft_putstr_fd(str, 0);
		while (++i < 100000)
		{
			j = 0;
			while (j < 30)
				j++;
		}
	}
    line.start.col = ft_strlen(launching) + 1;
    installing_files_2(line);
}

void    launch_(t_line line)
{
    int     i;

    i = 0;
    tputs(tgoto(tgetstr("SF", NULL), 0, line.winsz.row - 1), 1, &tc_putchar);
    check_updates("Checking for updates ...", line, NULL, 3);
    check_updates("No updates available", line, NULL, 0);
    installing_files_1("Installing files [", line, NULL);
    starting_ms("Starting minishell ...", line, NULL);
    tputs(tgoto(tgetstr("SF", NULL), 0, line.winsz.row - 1), 1, &tc_putchar);
    welcome_to_minishell("Welcome to minishell", line, NULL, -1);
    welcome_to_minishell("By rchallie and thervieu", line, NULL, 1);
}