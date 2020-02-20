/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thervieu <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/20 11:06:41 by thervieu          #+#    #+#             */
/*   Updated: 2019/11/20 11:06:43 by thervieu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list *current;
	t_list *next;

	if (lst != NULL && del != NULL && *del != NULL)
	{
		current = *lst;
		next = NULL;
		while (current != NULL)
		{
			next = current->next;
			ft_lstdelone(current, del);
			current = next;
		}
		*lst = NULL;
	}
}
