/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:41:59 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/21 11:19:38 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	g_signal_flag = 0;

// void	ft_printf_proofs_split_prs(t_data data)
// {
// 	int	count;

// 	count = 0;
// 	while (count < data.prs.how_much)
// 	{
// 		printf("string número %i es: %s\n", count, data.prs.arr_lexems[count]);
// 		count++;
// 	}
// }

void	header(void)
{
	printf("                ____\n");
	printf("               /\\   \\\n");
	printf("              /  \\___\\\n");
	printf("             _\\  / __/_\n");
	printf("            /\\ \\/_/\\   \\\n");
	printf("           /  \\__/  \\___\\\n");
	printf("          _\\  /  \\  / __/_\n");
	printf("         /\\ \\/___/\\/_/\\   \\\n");
	printf("        /  \\___\\    /  \\___\\\n");
	printf("       _\\  /   /_  _\\__/ __/_\n");
	printf("      /\\ \\/___/  \\/\\   \\/\\   \\\n");
	printf("     /  \\___\\ \\___\\ \\___\\ \\___\\\n");
	printf("     \\  /   / /   / /   / /   / \n");
	printf("      \\/___/\\/___/\\/___/\\/___/   \n");
	printf("\n");
	printf("░█░█░█▀▀░█░░░█▀▀░█▀█░█▄█░█▀▀░░░▀█▀░█▀█\n");
	printf("░█▄█░█▀▀░█░░░█░░░█░█░█░█░█▀▀░░░░█░░█░█\n");
	printf("░▀░▀░▀▀▀░▀▀▀░▀▀▀░▀▀▀░▀░▀░▀▀▀░░░░▀░░▀▀▀\n");
	printf("░█▄█░▀█▀░█▀█░▀█▀░█▀▀░█░█░█▀▀░█░░░█░░░█\n");
	printf("░█░█░░█░░█░█░░█░░▀▀█░█▀█░█▀▀░█░░░█░░░▀\n");
	printf("░▀░▀░▀▀▀░▀░▀░▀▀▀░▀▀▀░▀░▀░▀▀▀░▀▀▀░▀▀▀░▀\n");
	printf("\n");
	printf("\n");
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		exit_status;

	(void)argc;
	(void)argv;
	header();
	init_data(&data, env);
	disable_printing_signals(&data);
	signals_handler();
	while (1)
	{
		data.input = readline("MiniShell> ");
		if (data.input == NULL)
			break ;
		history(data.input);
		ft_parsing(&data);
		if (data.parsing_error == 0 && data.prs.flag_space == 'g')
		{
			execution(&data);
		}
		reboot_data(&data);
	}
	exit_status = data.exit_status;
	free_data(&data);
	printf("exit\n");
	return (exit_status);
}
