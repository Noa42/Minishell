#include "../../include/minishell.h"

int	g_signal_flag = 0;

void	ft_printf_proofs_split_prs(t_data data)
{
	int count;

	count = 0;
	while(count < data.prs.how_much)
	{
		printf("string número %i es: %s\n", count, data.prs.arr_lexems[count]);
		count++;
	}
	printf("data_input es: %s\n", data.input);
	printf("in_ax es: %s\n", data.in_ax);
}

int	main(int argc, char **argv, char **env)
{
	t_data	data;
	int		exit_status;

	(void)argc;
	(void)argv;
	init_data(&data, env);
	signals_handler();
	while (1)
	{
		data.input = readline("MiniShell> ");
		if (data.input == NULL)
			break ;
		history(data.input);
		ft_parsing(&data);
		if(data.exit_status == 0 && data.parsing_error == 0 && data.prs.flag_space == 'g')
		{
			//execution(&data);
			ft_printf_proofs_split_prs(data);
		}
		reboot_data(&data);
	}
	exit_status = data.exit_status; //esto es para que cuando hagamos ctrl+d el exit status sea el que corresponda
	free_data(&data);
	printf("exit\n");
	return (exit_status);
}
