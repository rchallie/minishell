/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rchallie <rchallie@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/27 10:23:19 by rchallie          #+#    #+#             */
/*   Updated: 2020/03/10 09:12:09 by rchallie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

int  is_spec_(char c)
{
    return (c == '\'' || c == '"' || c == '\\');
}

// static int has_redir(t_minishell *ms, int redir_type, int cursor, int fd)
// {
//     int O = 0;
//     int S = 0;
//     if (!ms->sequence[cursor] || ms->sequence[cursor] == 6
//         || ms->sequence[cursor] == 7)
//         return (fd);
//     else if (ms->sequence[cursor] == 2)
//         return (has_redir(ms, redir_type, ++cursor, fd));
//     else if (ms->sequence[cursor] == 3)
//         return (has_redir(ms, 3, ++cursor, fd));
//     else if (ms->sequence[cursor] == 8)
//     {
//         printf("LPOP\n");
//         if (redir_type == 0)
//             exit (123456789);
//         if (redir_type == 3)
//         {
//             O = O_CREAT | O_RDWR | O_TRUNC;
//             S = S_IRWXG | S_IRWXU | S_IRWXO;
//         }
//         else if (redir_type == 4)
//         {
//             O = O_CREAT | O_RDWR;
//             S = S_IRWXG | S_IRWXU | S_IRWXO;
//         }
//         if (!(fd = open(ms->treated[cursor], O, S)))
//         {
//             printf("Error overwrite : FD : %d\n", fd)	;
//             return (-1);
//         }
//     }
//     return (has_redir(ms, redir_type, ++cursor, fd));
// }

int		echo_(t_minishell *ms)
{
	int     cursor;
    int     len;
    int     has_opt;
    // int     fdoutput;

    cursor = ms->seq_cursor + 1;
    has_opt = (!ft_strcmp(ms->treated[cursor], "-n")) ? 1 : 0;
    cursor += has_opt;
    // if ((fdoutput = has_redir(ms , 0, cursor, 0)) == -1)
    //     exit(0);
    while (ms->sequence[cursor] == 2)
    {
        len = 0;
        while (len < (int)ft_strlen(ms->treated[cursor]))
            write(1, &ms->treated[cursor][len++], 1);
        cursor++;
    }
    if (!has_opt)
        write(1, "\n", 1);
    // if (fdoutput > 2)
    //     close(fdoutput);
    return (SUCCESS);
}
