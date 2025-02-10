/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HERE_DOC_redir.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:58:26 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/23 00:28:56 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*temp_file_name(t_cmd *cmd)
{
	char	*temp_file;
	char	*temp_join;
	char	*counter;

	counter = ft_itoa(cmd->data->here_doc_counter);
	temp_join = ft_strjoin(".temp_file_", counter);
	cmd->data->here_doc_counter++;
	temp_file = ft_strjoin(temp_join, ".txt");
	free(temp_join);
	free(counter);
	return (temp_file);
}

void	prompt_loop(int here_doc_fd, t_redir *redir)
{
	char	*line;
	int		i;

	i = 0;
	while (1)
	{
		line = readline("heredoc> ");
		if (line == NULL)
		{
			printf("warning: here-document at line %i \
				delimited by end-of-file (wanted `%s')\n", i, redir->delim);
			break ;
		}
		i++;
		if (has_variable(line) != 0)
			line = expand_vars_hd(line, redir->data);
		if (ft_strcmp(line, redir->delim) == 0)
			break ;
		write(here_doc_fd, line, ft_strlen(line));
		write(here_doc_fd, "\n", 1);
		free(line);
	}
	free(line);
}

void	delete_temp_file(t_cmd *cmd, char *temp_file)
{
	if (unlink(temp_file) == -1)
	{
		ft_putstr_fd("MiniShell: Error deleting temp file\n", 2);
		cmd->data->exit_status = 1;
		free(temp_file);
		return ;
	}
	free(temp_file);
}

void	apply_every_heredoc(t_data *data)
{
	t_cmd	*cmd;
	t_redir	*redir;

	cmd = data->cmd_list;
	while (cmd)
	{
		redir = cmd->redir_list;
		while (redir)
		{
			if (redir->type == HERE_DOC)
			{
				g_signal_flag = 2;
				signals_handler();
				apply_heredoc_redir(cmd, redir);
				g_signal_flag = 1;
				signals_handler();
			}
			redir = redir->next;
		}
		cmd = cmd->next;
	}
}

void	apply_heredoc_redir(t_cmd *cmd, t_redir *redir)
{
	char	*temp_file;

	temp_file = temp_file_name(cmd);
	cmd->fd_in = \
	open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (cmd->fd_in == -1)
	{
		ft_putstr_fd("MiniShell: Error opening file\n", 2);
		free(temp_file);
		cmd->data->exit_status = 1;
		return ;
	}
	prompt_loop(cmd->fd_in, redir);
	cmd->fd_in = open(temp_file, O_RDONLY);
	if (cmd->fd_in == -1)
	{
		ft_putstr_fd("MiniShell: Error opening file\n", 2);
		cmd->data->exit_status = 1;
		free(temp_file);
		return ;
	}
	delete_temp_file(cmd, temp_file);
	return ;
}
