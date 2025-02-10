/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:08 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 14:11:34 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_pwd(t_cmd *cmd)
{
	char	*dir;

	if (array_len(cmd->array_cmd) >= 2)
	{
		builtin_end(cmd->data, E2BIG);
		return ;
	}
	dir = malloc(sizeof(char) * 1024);
	if (dir == NULL)
	{
		ft_putstr_fd("MiniShell: Cannot allocate memory\n", 2);
		exit_process(cmd->data, 1);
	}
	if (getcwd(dir, 1024) != NULL)
		printf("%s\n", dir);
	free(dir);
	builtin_end(cmd->data, errno);
}
