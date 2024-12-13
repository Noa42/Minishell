#include "../../include/minishell.h"

void	update_last_command(t_cmd *cmd)
{
	insert_var(cmd->data->env, "_", cmd->array_cmd[0]);
	insert_var(cmd->data->array_var, "_", cmd->array_cmd[0]);
}
void	one_cmd_child(t_cmd* cmd, t_data *data)
{
	char	*path;

	dup_fds_redirs(cmd);
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		exit(cmd->data->exit_status);
	path = get_path(cmd->array_cmd[0], data->env);
	if (path != NULL)
		execve(path, cmd->array_cmd, data->env);
	ft_putstr_fd("Comand not found\n", 2);//esto solo ocurre si el execve falla
	exit(127);
}

void	one_cmd_case(t_data *data)
{
	t_cmd	*cmd;
	pid_t	pid;
	int		status;

	cmd = data->cmd_list;
	update_fds_redirs(data->cmd_list);
	if (is_a_builtin(cmd) == 1)
		one_builtin_case(cmd);
	else
	{
		pid = fork();
		if (pid == 0)
			one_cmd_child(cmd, cmd->data);
		else
		{
			waitpid(pid, &status, 0); //El exit status es una info que se tiene que interpretar con la macro WIFEXITED
			data->exit_status = WEXITSTATUS(status); //WEXITSTATUS es una macro que devuelve el exit status del hijo
		}
	}
	update_last_command(cmd);
}
void	one_builtin_case(t_cmd *cmd)
{
	int	stdin_copy;
	int	stdout_copy;

	stdin_copy = dup(STDIN_FILENO);
	stdout_copy = dup(STDOUT_FILENO);
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
	{
		close(stdin_copy);
		close(stdout_copy);
		return ;
	}
	dup_fds_redirs(cmd);
	exec_builtin(cmd);
	dup2(stdin_copy, STDIN_FILENO);
	dup2(stdout_copy, STDOUT_FILENO);
	close(stdin_copy);
	close(stdout_copy);
}
