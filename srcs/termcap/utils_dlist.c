/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_dlist.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/25 17:35:25 by thervieu          #+#    #+#             */
/*   Updated: 2020/02/25 17:35:27 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../incs/minishell.h"

t_dlist     *ft_dlst_new(void const *content, int content_size)
{
    t_dlist *new_link;

    if ((new_link = (t_dlist *)malloc(sizeof(t_dlist))) == NULL)
        return (NULL);
    ft_bzero(new_link, sizeof(t_dlist));
    if (!content)
    {
        new_link->content = NULL;
        new_link->content_size = 0;
    }
    else
    {
        if ((new_link->content = malloc(sizeof(content_size))) == NULL)
            return (NULL);
        new_link->content = ft_memcpy(new_link->content, content, content_size);
        new_link->content_size = content_size;
    }
    // write(1, "::end_new::\n", 12);
    return (new_link);
}

void    ft_dlst_add(t_dlist **head, t_dlist *new)
{
    t_dlist     *ptr;

    // write(1, "==beg_add==\n", 13);
    ptr = NULL;
    if (!head || !new)
        return ;
    new->next = (*head);
    if (*head)
    {
        ptr = (*head)->prev;
        (*head)->prev = new;
    }
    // write(1, "==af_fif==\n", 12);
    new->prev = ptr;
    // write(1, "==nprev = ptr==\n", 16);
    if (ptr)
    {
        // write(1, "AAA in if AAA\n", 13);
        ptr->next = new;
    }
    // write(1, "==end_add==\n", 13);
    *head = new;
}

int     ft_dlst_size(t_dlist *hist)
{
    int     len;

    len = 0;
    if (!hist)
        return (0);
    while (hist->next)
        hist = hist->next;
    while (hist)
    {
        hist = hist->prev;
        len++;
    }
    return (len);
}

void   ft_dlst_remove_link(t_dlist **head)
{
    t_dlist *ptr;

    if (*head)
    {
        if ((*head)->prev)
            (*head)->prev->next = (*head)->next;
        if ((*head)->next)
            (*head)->next->prev = (*head)->prev;
        ptr = *head;
        *head = (*head)->next ? (*head)->next : (*head)->prev;
        free(ptr->content);
        free(ptr);
    }
    return ;
}

void    ft_dlst_del(t_dlist **list)
{
    t_dlist *tmp;

    while (*list)
    {
        tmp = *list;
        free((*list)->content);
        *list = (*list)->next;
        free(tmp);
    }
    return ;
}