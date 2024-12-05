#include "../../include/minishell.h"

void	apply_APPEND_redir(t_cmd *cmd, t_redir *redir)
{
	cmd->fd_out = open(redir->out_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (cmd->fd_out == -1)
	{
		ft_putstr_fd("Error opening file\n", 2);
		cmd->data->exit_status = 1;
		return ;
	}
}
