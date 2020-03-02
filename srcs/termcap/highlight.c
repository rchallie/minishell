/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   highline.c                                        :+:      :+:    :+:   */
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

void    put_highline(t_highlight high)
{
    ft_printf("\e[49m\e[39m%s", high.beg);
    ft_printf("\e[47m\e[97m%s", high.hlight);
    ft_printf("\e[49m\e[39m%s\n", high.end);
    // ft_printf("beg = |%s|  hl = |%s|  end = |%s|", high.beg, high.hlight, high.end);
}


char    *get_string(t_line *line, int nb, int min, int max)
{
    int     size_hl;
    char    *str;

    size_hl = abs_diff(line->cursor, line->cursor_highl);
    min = (int)line->cursor < line->cursor_highl ? line->cursor : line->cursor_highl;
    max = (int)line->cursor > line->cursor_highl ? line->cursor : line->cursor_highl;
    // ft_printf("curs = %d  hl = %d\n", line->cursor, line->cursor_highl);
    if (nb == 0 && (str = malloc(sizeof(char) * (min + 1))) == NULL)
        return (NULL);
    if (nb == 1 && (str = malloc(sizeof(char) * (size_hl + 1))) == NULL)
        return (NULL);
    if (nb == 2 && (str = malloc(sizeof(char) * (ft_strlen(line->cmd) - max))) == NULL)
        return (NULL);
    if (nb == 0)
    {
        // write(1, "\n'<'\n", 5);
        // ft_printf("min = %d\n", min);
        str = ft_memcpy(str, line->cmd, min);
        // write(1, "\n''\n", 4);
        // ft_printf("min = %d\n", min);
        str[min] = '\0';
    }
    // write(1, "\n//\n", 4);
    if (nb == 1)
    {
        str = ft_memcpy(str, line->cmd + min, size_hl + 1);
        str[size_hl + 1] = '\0';
    }
    // write(1, "\n??\n", 4);
    if (nb == 2)
    {
        str = ft_memcpy(str, line->cmd + max + 1, ft_strlen(line->cmd) - max);
        str[ft_strlen(line->cmd) - max] = '\0';
    }
    return (str);
}

void    highlight(int key, t_line *line)
{
    t_highlight high;
    int     save;
    // if ((high = (t_highlight)malloc(sizeof(t_highlight))) == NULL)
    //     return ;
    if (key == KEY_SLEFT && line->cursor == 0)
        return ;
    if (key == KEY_SRIGHT && line->cursor == line->length)
        return ;
    if (line->cursor_highl == -1)
        line->cursor_highl = line->cursor;
    // ft_printf("curs = %d  hl = %d\n", line->cursor, line->cursor_highl);
    ft_bzero(&high, sizeof(highlight));
    if (key == KEY_SLEFT)
        line->cursor--;
    if (key == KEY_SRIGHT)
        line->cursor++;
    save = line->cursor;
    // write(1, "\nuu\n", 4);
    // write(1, "\nvv\n", 4);
    high.beg = get_string(line, 0, 0, 0);
    high.hlight = get_string(line, 1, 0, 0);
    high.end = get_string(line, 2, 0, 0);
    // write(1, "\n;;\n", 4);
    line->cursor = 0;
    set_curpos(line);
    put_highline(high);
    line->cursor = save;
    set_curpos(line);
    // ft_printf("\nbeg = |%s|  hl = |%s|  end = |%s|  str = |%s|  cursor = |%d|  hl_nb = |%d|", high.beg, high.hlight, high.end, line->cmd, line->cursor, line->cursor_highl);
    free(high.beg);
    free(high.hlight);
    free(high.end);
    return ;
}