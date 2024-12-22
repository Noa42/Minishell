/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:23 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 12:52:50 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	execution(t_data *data)
{
	g_signal_flag = 1;
	signals_handler();
	apply_every_heredoc(data);
	if (cmd_list_len(data->cmd_list) == 1)
		one_cmd_case(data);
	else
		multiple_cmd_case(data);
	printf("Exit status: %d\n", data->exit_status);
}
