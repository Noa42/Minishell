#include "../../include/minishell.h"

int	interact_mode = 0;

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
		//parsing(&data);
		//if (data.parsing_error == 0)
			execution(&data);
		reboot_data(&data);
	}
	exit_status = data.exit_status; //esto es para que cuando hagamos ctrl+d el exit status sea el que corresponda
	free_data(&data);
	printf("exit\n");
	return (exit_status);
}
