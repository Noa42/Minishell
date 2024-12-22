/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:56:53 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 14:11:24 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_env(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		printf("%s\n", array[i]);
		i++;
	}
}

void	ft_env(t_cmd *cmd)
{
	if (array_len(cmd->array_cmd) >= 2)
	{
		builtin_end(cmd->data, E2BIG);
		return ;
	}
	print_env(cmd->data->env);
	builtin_end(cmd->data, 0);
}
