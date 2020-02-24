/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_interr_term.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/20 19:37:14 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/20 19:37:15 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

static char term_buffer[2046];

void    init_terminal_data(void)
{
    char *termtype = getenv("TERM");
    int success;

    if (termtype == 0)
        ft_putstr("Specify a terminal type with `setenv TERM <yourtype>'.\n");

    success = tgetent(term_buffer, (const char *)termtype);
    if (success < 0)
        ft_putstr("Could not access the termcap data base.\n");
    if (success == 0)
        ft_putstr("Terminal type `%s' is not defined.\n");
}

// char *cl_string, *cm_string;
// int height;
// int width;
// int auto_wrap;

// char PC;   /* For tputs.  */
// char *BC;  /* For tgoto.  */
// char *UP;

// void    interrogate_terminal(void)
// {
//     char *buffer = (char *)malloc (ft_strlen(term_buffer));
//     char *temp;
// 	write(1, "xx\n", 3);		
//     /* Extract information we will use.  */
//     cl_string = tgetstr("cl", &buffer);
// 	write(1, "dd\n", 3);		
//     cm_string = tgetstr("cm", &buffer);
// 	write(1, "ll\n", 3);		
//     auto_wrap = tgetflag("am");
// 	write(1, "ll\n", 3);		
//     height = tgetnum("li");
// 	write(1, "ll\n", 3);		
//     width = tgetnum("co");
// 	write(1, "yy\n", 3);		

//     /* Extract information that termcap functions use.  */
//     temp = tgetstr("pc", &buffer);
//     PC = temp ? *temp : 0;
//     BC = tgetstr("le", &buffer);
//     UP = tgetstr("up", &buffer);
// 	write(1, "zz\n", 3);		
// }