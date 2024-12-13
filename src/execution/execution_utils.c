#include "../../include/minishell.h"

void	exit_process(t_data *data, int exit_status)
{
	free_data(data);
	exit(exit_status);
}

void	close_fds(void)
{
	int	i;

	i = 3; //el primer fd no estandard es el 3
	while (i < 1024)//1024 es el maximo numero de fd que se pueden tener
	{
		close(i);
		i++;
	}
}
