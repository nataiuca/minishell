/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 23:06:54 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/26 23:06:54 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*rslt;
	t_list	*tmp;
	void	*content;

	if (!lst || !f || !del)
		return (NULL);
	rslt = NULL;
	while (lst != NULL)
	{
		content = (*f)(lst->content);
		tmp = ft_lstnew(content);
		if (tmp == NULL)
		{
			(*del)(content);
			ft_lstclear(&rslt, (*del));
			return (rslt);
		}
		ft_lstadd_back(&rslt, tmp);
		lst = lst->next;
	}
	return (rslt);
}
