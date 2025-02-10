/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   APPEND_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:58:19 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 10:28:40 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	apply_append_redir(t_cmd *cmd, t_redir *redir)
{
	cmd->fd_out = open(redir->out_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->fd_out == -1)
	{
		ft_putstr_fd("MiniShell: Error opening file\n", 2);
		cmd->data->exit_status = 1;
		return ;
	}
}
