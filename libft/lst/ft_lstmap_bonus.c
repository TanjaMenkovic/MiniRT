/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmenkovi <tmenkovi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/27 16:18:36 by tmenkovi          #+#    #+#             */
/*   Updated: 2024/08/27 16:18:37 by tmenkovi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/libft.h"

static t_list	*issue(void (*del)(void *), t_list *first, void *cnt);

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*first;
	t_list	*node;
	void	*cnt;

	if (!lst || !f || !del)
		return (NULL);
	cnt = f(lst->content);
	node = ft_lstnew(cnt);
	if (!node)
	{
		del(cnt);
		return (NULL);
	}
	first = node;
	lst = lst->next;
	while (lst)
	{
		cnt = (*f)(lst->content);
		node->next = ft_lstnew(cnt);
		if (!(node->next))
			return (issue(del, first, cnt));
		node = node->next;
		lst = lst->next;
	}
	return (first);
}

static t_list	*issue(void (*del)(void *), t_list *first, void *cnt)
{
	del(cnt);
	ft_lstclear(&first, del);
	return (NULL);
}
