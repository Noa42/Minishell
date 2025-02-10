/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multiple_cmd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:26 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 11:30:05 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	multiple_cmd_case(t_data *data)
{
	t_cmd	*cmd;
	int		fd_in;
	int		fd_out;
	pid_t	pid;
	int		status;

	fd_out = STDOUT_FILENO;
	fd_in = STDIN_FILENO;
	cmd = data->cmd_list;
	while (cmd)
	{
		pipe(data->pipe);
		safe_fork(&pid, data);
		if (pid == 0)
			child(cmd, &fd_in, &fd_out, data);
		else
		{
			waitpid(pid, &status, 0);
			data->exit_status = WEXITSTATUS(status);
			close(data->pipe[1]);
			fd_in = data->pipe[0];
			cmd = cmd->next;
		}
	}
}

void	child(t_cmd *cmd, int *fd_in, int *fd_out, t_data *data)
{
	if (is_last_cmd(cmd) == 0)
		*fd_out = data->pipe[1];
	else
		*fd_out = STDOUT_FILENO;
	safe_dup2(fd_in, fd_out, data);
	close(data->pipe[0]);
	close(data->pipe[1]);
	if (update_fds_redirs(cmd) != 0)
	{
		data->exit_status = 1;
		exit_process(data, data->exit_status);
	}
	dup_fds_redirs(cmd);
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		exit_process(data, data->exit_status);
	if (is_a_builtin(cmd) == 1)
	{
		exec_builtin(cmd);
		exit_process(data, data->exit_status);
	}
	else
		exec_cmd(cmd, data);
}
