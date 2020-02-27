/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   underline.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 14:21:50 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/27 14:21:51 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int     abs_diff(int a, int b)
{
    if (a <= b)
        return (b - a);
    return (a - b);
}

void    put_underline(char *str, t_line *line)
{
    int     min;
    int     max;
    int     save;
    char    *beg_str;

    (void)str;
    beg_str = (char *)line->cmd;
    // ft_printf("%s\n", beg_str);
    save = line->cursor;
    min = (int)line->cursor < line->cursor_underl ? line->cursor : line->cursor_underl;
    max = (int)line->cursor < line->cursor_underl ? line->cursor_underl : line->cursor;
    line->cursor = 0;
    beg_str[min - 1] = '\0';
    tputs(tgetstr("cd", NULL), 0, &tc_putchar);
    ft_putstr_fd("\e[49m\e[39", 0);
    ft_putstr_fd(beg_str, 0);
    line->cursor = min;
    set_curpos(line);
    // ft_printf("\e[47m\e[97m%s\e[49m\e[39m%s", str, line->cmd + max);
    line->cursor = save;
    // ft_printf("\n\nstr = |%s|", str);
    set_curpos(line);
}

void    under_left(t_line *line)
{
    int     size_underl;
    char    *str;
    // ft_printf("aa\n");
    size_underl = abs_diff(line->cursor, line->cursor_underl);
    if (line->cursor == 0)
        return ;
    if (line->cursor_underl == -1)
        line->cursor_underl = line->cursor;
    // write(1, "a\n", 2);
    // ft_printf("ul = %d\n", line->cursor_underl);
    line->cursor--;
    // write(1, "b\n", 2);
    if ((str = malloc(sizeof(char) * (size_underl + 1))) == NULL)
        return ;
    str = ft_memcpy(str, line->cmd + line->cursor, size_underl);
    str[size_underl] = '\0';
    // write(1, "c\n", 2);
    put_underline(str, line);
    // write(1, "end\n", 4);
    free(str);
    return ;
}

void    under_right(t_line *line)
{
    int     size_underl;
    char    *str;
    // ft_printf("aa\n");
    size_underl = abs_diff(line->cursor, line->cursor_underl);
    if (line->cursor == line->length)
        return ;
    if (line->cursor_underl == -1)
        line->cursor_underl = line->cursor;
    // write(1, "a", 1);
    // ft_printf("ul = %d\n", line->cursor_underl);
    line->cursor++;
    if ((str = malloc(sizeof(char) * (size_underl + 1))) == NULL)
        return ;
    str = ft_memcpy(str, line->cmd + line->cursor, size_underl);
    str[size_underl] = '\0';
    set_curpos(line);
    put_underline(str, line);
    free(str);
    return ;
}