/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_execution.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:56:24 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 10:19:50 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_a_builtin(t_cmd *cmd)
{
	if ((ft_strcmp(cmd->array_cmd[0], "echo") == 0)
		|| (ft_strcmp(cmd->array_cmd[0], "pwd") == 0)
		|| (ft_strcmp(cmd->array_cmd[0], "cd") == 0)
		|| (ft_strcmp(cmd->array_cmd[0], "env") == 0)
		|| (ft_strcmp(cmd->array_cmd[0], "export") == 0)
		|| (ft_strcmp(cmd->array_cmd[0], "unset") == 0)
		|| (ft_strcmp(cmd->array_cmd[0], "exit") == 0))
		return (1);
	else
		return (0);
}

void	exec_builtin(t_cmd *cmd)
{
	if (ft_strcmp(cmd->array_cmd[0], "echo") == 0)
		ft_echo(cmd);
	else if (ft_strcmp(cmd->array_cmd[0], "pwd") == 0)
		ft_pwd(cmd);
	else if (ft_strcmp(cmd->array_cmd[0], "cd") == 0)
		ft_cd(cmd);
	else if (ft_strcmp(cmd->array_cmd[0], "env") == 0)
		ft_env(cmd);
	else if (ft_strcmp(cmd->array_cmd[0], "export") == 0)
		ft_export(cmd);
	else if (ft_strcmp(cmd->array_cmd[0], "unset") == 0)
		ft_unset(cmd);
	else if (ft_strcmp(cmd->array_cmd[0], "exit") == 0)
		ft_exit(cmd);
}

void	builtin_end(t_data *data, int error)
{
	if (error == 0)
	{
		data->exit_status = 0;
	}
	else
	{
		ft_putstr_fd("MiniShell: ", 2);
		ft_putstr_fd(strerror(error), 2);
		ft_putstr_fd("\n", 2);
		data->exit_status = error;
	}
}
