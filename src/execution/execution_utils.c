/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:20 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 15:51:12 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	exit_process(t_data *data, int exit_status)
{
	free_data(data);
	exit(exit_status);
}

void	close_fds(void)
{
	int	i;

	i = 3;
	while (i < 1024)
	{
		close(i);
		i++;
	}
}

void	safe_fork(pid_t *pid, t_data *data)
{
	*pid = fork();
	if (*pid == -1)
	{
		ft_putstr_fd("MiniShell: Fork error\n", 2);
		exit_process(data, 1);
	}
}

void	safe_dup2(int *fd_in, int *fd_out, t_data *data)
{
	if (dup2(*fd_in, STDIN_FILENO) == -1 || dup2(*fd_out, STDOUT_FILENO) == -1)
	{
		ft_putstr_fd("MiniShell: Dup error\n", 2);
		data->exit_status = 1;
		exit_process(data, data->exit_status);
	}
}

void	exec_cmd(t_cmd *cmd, t_data *data)
{
	char	*path;

	path = get_path(cmd->array_cmd[0], data->env);
	if (path != NULL)
		execve(path, cmd->array_cmd, data->env);
	ft_putstr_fd("MiniShell: ", 2);
	if (ft_strcmp(cmd->array_cmd[0], " ") != 0)
		ft_putstr_fd(cmd->array_cmd[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	exit_process(data, 127);
}
