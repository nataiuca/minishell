/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgamarra <jgamarra@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/26 23:06:46 by jgamarra          #+#    #+#             */
/*   Updated: 2024/01/26 23:06:46 by jgamarra         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(void *content)
{
	t_list	*rslt;

	rslt = (t_list *)malloc(sizeof(t_list));
	if (rslt == NULL)
		return (NULL);
	rslt->content = content;
	rslt->next = NULL;
	return (rslt);
}
