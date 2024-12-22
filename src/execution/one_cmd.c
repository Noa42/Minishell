/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   one_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:35 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 11:30:26 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	update_last_command(t_cmd *cmd)
{
	insert_var(cmd->data->env, "_", cmd->array_cmd[0]);
	insert_var(cmd->data->array_var, "_", cmd->array_cmd[0]);
}

void	one_cmd_child(t_cmd *cmd, t_data *data)
{
	if (update_fds_redirs(cmd) != 0)
	{
		data->exit_status = 1;
		exit_process(data, data->exit_status);
	}
	dup_fds_redirs(cmd);
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		exit(cmd->data->exit_status);
	exec_cmd(cmd, data);
}

void	one_cmd_case(t_data *data)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		status;

	cmd = data->cmd_list;
	if (is_a_builtin(cmd) == 1)
		one_builtin_case(cmd);
	else
	{
		safe_fork(&pid, data);
		if (pid == 0)
			one_cmd_child(cmd, cmd->data);
		else
		{
			waitpid(pid, &status, 0);
			data->exit_status = WEXITSTATUS(status);
		}
	}
	if (data->exit_status == 0)
		update_last_command(cmd);
}

void	one_builtin_case(t_cmd *cmd)
{
	int	stdin_copy;
	int	stdout_copy;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
	{
		close(stdin_copy);
		close(stdout_copy);
		return ;
	}
	if (update_fds_redirs(cmd) != 0)
	{
		close(stdin_copy);
		close(stdout_copy);
		cmd->data->exit_status = 1;
		return ;
	}
	dup_fds_redirs(cmd);
	exec_builtin(cmd);
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}
