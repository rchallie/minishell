/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 10:23:19 by rchallie          #+#    #+#             */
/*   Updated: 2020/02/27 10:24:18 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int  is_spec_(char c)
{
    return (c == '\'' || c == '"' || c == '\\');
}

int		echo_(t_minishell *ms)
{
	int     cursor;
    int     len;
    int     has_opt;

    cursor = ms->seq_cursor + 1;
    has_opt = (!ft_strcmp(ms->treated[cursor], "-n")) ? 1 : 0;
    cursor += has_opt;
    while (ms->sequence[cursor] == 2)
    {
        len = 0;
        while (len < (int)ft_strlen(ms->treated[cursor]))
            write(1, &ms->treated[cursor][len++], 1);
        cursor++;
    }
    if (!has_opt)
        write(1, "\n", 1);
    return (SUCCESS);
}
