/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_execution.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:42:42 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/20 13:22:09 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"


void	apply_last_in_redir(t_cmd *cmd)
{
	t_redir	*last_in_redir;

	last_in_redir = get_last_in_redir(cmd);
	if (last_in_redir != NULL)
	{
		if (last_in_redir->type == INPUT)
			apply_input_redir(cmd, last_in_redir);
	}
	else
		cmd->fd_in = 0;
}

void	apply_last_out_redir(t_cmd *cmd)
{
	t_redir	*last_out_redir;

	last_out_redir = get_last_out_redir(cmd);
	if (last_out_redir != NULL)
	{
		if (last_out_redir->type == OUTPUT)
			apply_output_redir(cmd, last_out_redir);
		else if (last_out_redir->type == APPEND)
			apply_append_redir(cmd, last_out_redir);
	}
	else
		cmd->fd_out = 1;
}

int	open_and_try_redir(t_redir *redir)
{
	int	fd;

	if (redir->type == INPUT)
	{
		fd = open(redir->in_name, O_RDONLY);
		if (fd == -1)
		{
			ft_putstr_fd(redir->in_name, 2);
			ft_putstr_fd(": Error opening file\n", 2);
			return (1);
		}
		close(fd);
	}
	else if (redir->type == OUTPUT || redir->type == APPEND)
	{
		fd = open(redir->out_name, O_WRONLY | O_CREAT, 0644);
		if (fd == -1)
		{
			ft_putstr_fd(redir->out_name, 2);
			ft_putstr_fd(": Error opening file\n", 2);
			return (1);
		}
		close(fd);
	}
	return (0);
}

int	update_fds_redirs(t_cmd *cmd)
{
	t_redir	*redir;
	int		result;

	result = 0;
	redir = cmd->redir_list;
	while (redir)
	{
		result += open_and_try_redir(redir);
		if (result != 0)
			break ;
		redir = redir->next;
	}
	if (result == 0)
	{
		apply_last_in_redir(cmd);
		apply_last_out_redir(cmd);
	}
	return (result);
}

void	dup_fds_redirs(t_cmd *cmd) //Esto es para un solo comando
{
	if (cmd->fd_in != 0 && cmd->fd_in != -1)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			ft_putstr_fd("Error duplicating file descriptor\n", 2);
			cmd->data->exit_status = 1;
			return ;
		}
	}
	if (cmd->fd_out != 1 && cmd->fd_out != -1)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("Error duplicating file descriptor\n", 2);
			cmd->data->exit_status = 1;
			return ;
		}
	}
}
