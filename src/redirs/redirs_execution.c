#include "../../include/minishell.h"

void	apply_redir_list(t_cmd *cmd)
{
	if (cmd->redir_list == NULL)
		return ;
	apply_last_in_redir(cmd);
	apply_last_out_redir(cmd);
	if (cmd->fd_in == -1 || cmd->fd_out == -1)
		return ;
	if (cmd->fd_in != 0)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			ft_printf("Error duplicating file descriptor\n", 2);
			cmd->data->exit_status = 1;
			return ;
		}
	}
	if (cmd->fd_out != 1)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			ft_printf("Error duplicating file descriptor\n", 2);
			cmd->data->exit_status = 1;
			return ;
		}
	}
}

void	apply_last_in_redir(t_cmd *cmd)
{
	t_redir	*last_in_redir;

	last_in_redir = get_last_in_redir(cmd);
	if (last_in_redir != NULL)
	{
		if (last_in_redir->type == INPUT)
			apply_INPUT_redir(cmd, last_in_redir);
		else if (last_in_redir->type == HERE_DOC)
		{
			g_signal_flag = 2;
			signals_handler();
			apply_HERE_DOC_redir(cmd, last_in_redir);
			g_signal_flag = 1;
			signals_handler();
		}
	}
	else
		cmd->fd_in = 0;
}

void	apply_last_out_redir(t_cmd *cmd)
{
	t_redir	*last_out_redir;

	last_out_redir = get_last_out_redir(cmd);
	if (last_out_redir != NULL)
	{
		if (last_out_redir->type == OUTPUT)
			apply_OUTPUT_redir(cmd, last_out_redir);
		else if (last_out_redir->type == APPEND)
			apply_APPEND_redir(cmd, last_out_redir);
	}
	else
		cmd->fd_out = 1;
}

void	update_fds_redirs(t_cmd *cmd_list) //esto es para uno o varios comandos
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->redir_list != NULL)
		{
			apply_last_in_redir(cmd);
			apply_last_out_redir(cmd);
		}
		cmd = cmd->next;
	}
}

void	dup_fds_redirs(t_cmd *cmd) //Esto es para un solo comando
{
	if (cmd->fd_in != 0)
	{
		if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
		{
			ft_putstr_fd("Error duplicating file descriptor\n", 2);
			cmd->data->exit_status = 1;
			return ;
		}
	}
	if (cmd->fd_out != 1)
	{
		if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
		{
			ft_putstr_fd("Error duplicating file descriptor\n", 2);
			cmd->data->exit_status = 1;
			return ;
		}
	}
}
