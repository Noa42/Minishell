/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd_list_utils_1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:44 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 10:28:00 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_cmd	*new_cmd(char **array_cmds, t_data *data)
{
	t_cmd	*new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	if (new_cmd == NULL)
	{
		ft_putstr_fd("MiniShell: Cannot allocate memory\n", 2);
		exit_process(data, 1);
	}
	new_cmd->array_cmd = array_cmds;
	new_cmd->fd_in = STDIN_FILENO;
	new_cmd->fd_out = STDOUT_FILENO;
	new_cmd->redir_list = NULL;
	new_cmd->data = data;
	new_cmd->next = NULL;
	return (new_cmd);
}

t_cmd	*add_cmd(t_cmd *cmd_list, t_cmd *new_cmd)
{
	t_cmd	*p;

	if (cmd_list == NULL)
	{
		new_cmd->index = 0;
		return (new_cmd);
	}
	p = cmd_list;
	while (p->next)
		p = p->next;
	p->next = new_cmd;
	new_cmd->index = cmd_list_len(cmd_list)- 1;
	return (cmd_list);
}

int	cmd_list_len(t_cmd *cmd_list)
{
	t_cmd	*p;
	int		len;

	len = 0;
	p = cmd_list;
	if (p == NULL)
		return (0);
	while (p)
	{
		len++;
		p = p->next;
	}
	return (len);
}

t_cmd	*free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*p;
	t_cmd	*tmp;

	p = cmd_list;
	while (p)
	{
		tmp = p->next;
		if (p->redir_list)
			p->redir_list = free_redir_list(p->redir_list);
		free_cmd(p);
		p = tmp;
	}
	return (NULL);
}

void	free_cmd(t_cmd *cmd)
{
	if (cmd->array_cmd)
		free_array(cmd->array_cmd);
	if (cmd->redir_list)
		free_redir_list(cmd->redir_list);
	if (cmd->fd_in != STDIN_FILENO && cmd->fd_in != -1)
		close(cmd->fd_in);
	if (cmd->fd_out != STDOUT_FILENO && cmd->fd_out != -1)
		close(cmd->fd_out);
	free(cmd);
	cmd = NULL;
}
