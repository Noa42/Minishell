/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   INPUT_redir.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:58:29 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 10:29:29 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	apply_input_redir(t_cmd *cmd, t_redir *redir)
{
	cmd->fd_in = open(redir->in_name, O_RDONLY);
	if (cmd->fd_in == -1)
	{
		ft_putstr_fd("MiniShell: Error opening file\n", 2);
		cmd->data->exit_status = 1;
		return ;
	}
}
