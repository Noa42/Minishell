/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:42:49 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 11:34:00 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_data	*get_data(t_data *data)
{
	static t_data	*data_ptr;

	if (data_ptr == NULL)
		data_ptr = data;
	return (data_ptr);
}

void	handle_backlash(int sign)
{
	(void)sign;
	if (g_signal_flag == 1)
		printf("Quit (core dumped) \n");
}

void	handle_ctrl_c(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 1);
	rl_on_new_line();
	if (g_signal_flag == 0)
		rl_redisplay();
	get_data(NULL)->exit_status = 130;
}

void	signals_heredoc(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGTSTP, SIG_IGN);
}

void	signals_handler(void)
{
	if (g_signal_flag == 0)
	{
		signal(SIGINT, handle_ctrl_c);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
	if (g_signal_flag == 1)
	{
		signal(SIGINT, handle_ctrl_c);
		signal(SIGQUIT, handle_backlash);
		signal(SIGTSTP, SIG_IGN);
	}
	if (g_signal_flag == 2)
	{
		signal(SIGINT, SIG_IGN);
		signal(SIGQUIT, SIG_IGN);
		signal(SIGTSTP, SIG_IGN);
	}
}
