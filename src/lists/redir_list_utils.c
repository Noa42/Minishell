/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_list_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:41:51 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 10:27:55 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*new_redir(t_redir_type type, char *input, t_data *data)
{
	t_redir	*new_redir;

	new_redir = malloc(sizeof(t_redir));
	if (new_redir == NULL)
		return (NULL);
	new_redir->type = type;
	new_redir->in_name = NULL;
	new_redir->out_name = NULL;
	new_redir->delim = NULL;
	if (type == INPUT)
		new_redir->in_name = input;
	else if (type == OUTPUT || type == APPEND)
		new_redir->out_name = input;
	else if (type == HERE_DOC)
		new_redir->delim = input;
	new_redir->data = data;
	new_redir->next = NULL;
	return (new_redir);
}

void	update_index_redir_list(t_redir *redir_list)
{
	t_redir	*p;
	int		i;

	i = 0;
	p = redir_list;
	if (p == NULL)
		return ;
	while (p)
	{
		p->index = i;
		i++;
		p = p->next;
	}
}

void	add_redir(t_cmd *cmd, t_redir *redir)
{
	t_redir	*p;

	if (redir == NULL)
	{
		ft_putstr_fd("MiniShell: Cannot allocate memory\n", 2);
		exit_process(cmd->data, 1);
	}
	if (cmd->redir_list == NULL)
	{
		cmd->redir_list = redir;
		return ;
	}
	p = cmd->redir_list;
	while (p->next)
		p = p->next;
	p->next = redir;
	update_index_redir_list(cmd->redir_list);
}

void	free_redir(t_redir redir)
{
	if (redir.in_name)
		free(redir.in_name);
	if (redir.out_name)
		free(redir.out_name);
	if (redir.delim)
		free(redir.delim);
}

t_redir	*free_redir_list(t_redir *redir_list)
{
	t_redir	*p;
	t_redir	*tmp;

	p = redir_list;
	while (p)
	{
		tmp = p->next;
		free_redir(*p);
		free(p);
		p = tmp;
	}
	return (NULL);
}
