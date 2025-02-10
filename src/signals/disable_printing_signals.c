/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   disable_printing_signals.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:45:30 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/19 11:13:55 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	restore_original_settings(t_data *data)
{
	tcsetattr(STDIN_FILENO, TCSANOW, &data->original_settings);
}

void	disable_printing_signals(t_data *data)
{
	struct termios	new_settings;

	tcgetattr(STDIN_FILENO, &data->original_settings);
	new_settings = data->original_settings;
	new_settings.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);
}
