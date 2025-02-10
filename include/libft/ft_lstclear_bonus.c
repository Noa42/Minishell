/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/20 16:19:06 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 12:37:51 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstclear(t_stack **lst, void (*del)(void*))
{
	t_stack	*ptr;

	while (*lst != NULL)
	{
		ptr = *lst;
		*lst = (*lst)->next;
		del (ptr -> content);
		free (ptr);
	}
}
