/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:23 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/23 01:35:13 by alvapari         ###   ########.fr       */
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
}
