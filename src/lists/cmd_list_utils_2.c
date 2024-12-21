/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_utils_2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:48 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/21 11:16:01 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*get_last_cmd(t_cmd *cmd_list)
{
	t_cmd	*p;

	p = cmd_list;
	if (p == NULL)
		return (NULL);
	while (p->next)
		p = p->next;
	return (p);
}

t_cmd	*get_cmd_by_index(t_cmd *cmd_list, int index)
{
	t_cmd	*p;

	p = cmd_list;
	if (p == NULL)
		return (NULL);
	while (p)
	{
		if (p->index == index)
			return (p);
		p = p->next;
	}
	return (NULL);
}

int	is_last_cmd(t_cmd *cmd)
{
	if (cmd->next == NULL)
		return (1);
	return (0);
}

int	is_first_cmd(t_cmd *cmd)
{
	if (cmd->index == 0)
		return (1);
	return (0);
}
