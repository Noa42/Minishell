/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:01 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 11:28:17 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_a_num(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-')
		i++;
	while (str[i])
	{
		if (ft_isdigit(str[i]) == 0)
			return (0);
		i++;
	}
	return (1);
}

int	i_to_exit_status(int exit_status)
{
	if (exit_status < 0)
		exit_status = 256 + exit_status;
	if (exit_status > 255)
		exit_status = exit_status % 256;
	return (exit_status);
}

void	ft_exit(t_cmd *cmd)
{
	int	exit_status;

	printf("exit");
	if (array_len(cmd->array_cmd) == 1)
		exit_status = 0;
	else if ((array_len(cmd->array_cmd) == 2)
		|| (array_len(cmd->array_cmd) > 2 && is_a_num(cmd->array_cmd[1]) == 0))
	{
		if (is_a_num(cmd->array_cmd[1]) == 0)
		{
			printf(": %s: numeric argument required", cmd->array_cmd[1]);
			exit_status = 2;
		}
		else
			exit_status = i_to_exit_status(ft_atoi(cmd->array_cmd[1]));
	}
	else if (is_a_num(cmd->array_cmd[1]) == 1)
	{
		builtin_end(cmd->data, E2BIG);
		printf("\n");
		return ;
	}
	printf("\n");
	free_data(cmd->data);
	exit(exit_status);
}
