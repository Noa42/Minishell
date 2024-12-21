/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:58:03 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/21 11:17:35 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_token	*add_token(t_token *token_list, t_token_type token_type)
{
	t_token	*new_token;
	t_token	*p;

	new_token = malloc(sizeof(t_token));
	if (new_token == NULL)
		return (NULL);
	new_token->token_type = token_type;
	new_token->next = NULL;
	if (token_list == NULL)
	{
		new_token->index = 0;
		return (new_token);
	}
	p = token_list;
	while (p->next)
		p = p->next;
	p->next = new_token;
	new_token->index = token_list_len(token_list) - 1;
	return (token_list);
}

int	token_list_len(t_token *token_list)
{
	t_token	*p;
	int		len;

	len = 0;
	p = token_list;
	if (p == NULL)
		return (0);
	while (p)
	{
		len++;
		p = p->next;
	}
	return (len);
}

t_token	*free_token_list(t_token *token_list)
{
	t_token	*p;
	t_token	*tmp;

	p = token_list;
	while (p)
	{
		tmp = p->next;
		free(p);
		p = tmp;
	}
	return (NULL);
}
