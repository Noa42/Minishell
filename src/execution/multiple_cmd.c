#include "../../include/minishell.h"

void	multiple_cmd_case(t_data *data)
{
	t_cmd	*cmd;    
	int		fd_in;
	int		fd_out;
	pid_t	pid;
	int		status;

	fd_out = STDOUT_FILENO;// Descriptor de archivo para la salida del último comando
	fd_in = STDIN_FILENO; // Descriptor de archivo para la entrada del primer comando
	cmd = data->cmd_list;
	//update_fds_redirs(data->cmd_list);
	while (cmd)
	{
		pipe(data->pipe); // Crea la tubería para el comando actual y el siguiente
		safe_fork(&pid, data);
		if (pid == 0) //Hijo
			child(cmd, &fd_in, &fd_out, data);
		else//Padre
		{
			waitpid(pid, &status, 0); //El exit status es una info que se tiene que interpretar con la macro WIFEXITED
			data->exit_status = WEXITSTATUS(status); //WEXITSTATUS es una macro que devuelve el exit status del hijo
			close(data->pipe[1]); // Cierra el extremo de escritura de la tubería en el padre
			fd_in = data->pipe[0];			// Actualiza fd_in para el próximo comando, usando el extremo de lectura actual
			cmd = cmd->next;
		}
	}
}

void	child(t_cmd *cmd, int *fd_in, int *fd_out, t_data *data)
{
	// Configura la salida del proceso hijo
	if (is_last_cmd(cmd) == 0) // Si hay un comando siguiente
		*fd_out = data->pipe[1]; // Redirige la salida a la escritura de la tubería
	else
		*fd_out = STDOUT_FILENO; // Si es el último comando, redirige a la salida estándar
	safe_dup2(fd_in, fd_out, data);
	close(data->pipe[0]); // Cierra el extremo de lectura de la tubería en el hijo
	close(data->pipe[1]); // Cierra el extremo de escritura de la tubería en el hijo
	if (update_fds_redirs(cmd) != 0)
	{
		data->exit_status = 1;
		exit_process(data, data->exit_status);
	}
	dup_fds_redirs(cmd);
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		exit_process(data, data->exit_status);
	if (is_a_builtin(cmd) == 1)
	{
		exec_builtin(cmd);
		exit_process(data, data->exit_status);
	}
	else
		exec_cmd(cmd, data);
}
