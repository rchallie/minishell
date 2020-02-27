/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 14:14:01 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/25 14:14:02 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

void        content_params(t_line *line, char *str)
{
    cursor_to_home(line);
    tputs(tgetstr("cd", NULL), 0, &tc_putchar);
    ft_bzero(line->cmd, MAX_CMD_LEN);
    ft_strcpy(line->cmd, str);
    line->length = ft_strlen(line->cmd);
    line->cursor = line->length;
    ft_putstr_fd(line->cmd, 0);
}

void        new_history(t_line *line, t_dlist **hist)
{
    if (!line->hist_depth)
        return ;
    *hist = (*hist)->prev;
    line->hist_depth--;
    content_params(line, (*hist)->content);
    if (!line->hist_depth)
        ft_dlst_remove_link(hist);
}

void        old_history(t_line *line, t_dlist **hist)
{
    if (line->hist_depth == line->hist_size)
        return ;
    if (!line->hist_depth)
        ft_dlst_add(hist, ft_dlst_new(line->cmd, ft_strlen(line->cmd) + 1));
    *hist = (*hist)->next;
    line->hist_depth++;
    content_params(line, (*hist)->content);
}

void        append_history(char *new_hist)
{
    int     fd;
    mode_t mode;

    mode = S_IRUSR | S_IWUSR;
    if (*new_hist =='\0')
        return ;
    if ((fd = open(HISTORY_PATH, O_WRONLY | O_APPEND | O_CREAT, mode)) == -1)
        return ;
    ft_putendl_fd(new_hist, fd);
    close(fd);
    return ;
}

t_dlist     *get_history(void)
{
    int     fd;
    int     len;
    t_dlist *hist;
    char    *line;

    if ((fd = open(HISTORY_PATH, O_RDONLY)) == -1)
        return (NULL);
    if (!(hist = malloc(sizeof(t_dlist))))
        return (NULL);
    ft_bzero(hist, sizeof(t_dlist));
    line = NULL;
    // hist = NULL;
    // hist->prev = NULL;
    while (get_next_line(fd, &line))
    {
        len = ft_strlen(line);
        // ft_printf("line = |%s|\n", line);
        // ft_printf("len = |%d|\n", len);
        if (len < MAX_CMD_LEN)
            ft_dlst_add(&hist, ft_dlst_new(line, len + 1));
        // free(line);
        // write(1, "||got_line||\n\n\n\n\n", 13);
        line = NULL;
    }
    // write(1, "{{got_file}}\n", 13);
    // free(line);
    close(fd);
    return (hist);
}
