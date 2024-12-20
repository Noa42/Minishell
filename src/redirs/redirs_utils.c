/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:58:38 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/21 11:21:45 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*get_last_out_redir(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*last_out_redir;

	last_out_redir = NULL;
	redir = cmd->redir_list;
	while (redir)
	{
		if (redir->type == OUTPUT || redir->type == APPEND)
			last_out_redir = redir;
		redir = redir->next;
	}
	return (last_out_redir);
}

t_redir	*get_last_in_redir(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*last_in_redir;

	last_in_redir = NULL;
	redir = cmd->redir_list;
	while (redir)
	{
		if (redir->type == INPUT || redir->type == HERE_DOC)
			last_in_redir = redir;
		redir = redir->next;
	}
	return (last_in_redir);
}
