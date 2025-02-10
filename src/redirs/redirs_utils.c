/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirs_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:58:38 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 16:18:15 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_redir	*get_last_out_redir(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*last_out_redir;

	last_out_redir = NULL;
	redir = cmd->redir_list;
	while (redir)
	{
		if (redir->type == OUTPUT || redir->type == APPEND)
			last_out_redir = redir;
		redir = redir->next;
	}
	return (last_out_redir);
}

t_redir	*get_last_in_redir(t_cmd *cmd)
{
	t_redir	*redir;
	t_redir	*last_in_redir;

	last_in_redir = NULL;
	redir = cmd->redir_list;
	while (redir)
	{
		if (redir->type == INPUT || redir->type == HERE_DOC)
			last_in_redir = redir;
		redir = redir->next;
	}
	return (last_in_redir);
}

void	print_error_openining_file(char *file_name)
{
	ft_putstr_fd("MiniShell: ", 2);
	ft_putstr_fd(file_name, 2);
	ft_putstr_fd(": Error opening file\n", 2);
}

int	open_and_try_redir_append(t_redir *redir)
{
	int	fd;

	fd = open(redir->out_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		print_error_openining_file(redir->out_name);
		return (1);
	}
	close(fd);
	return (0);
}

int	open_and_try_redir_output(t_redir *redir)
{
	int	fd;

	fd = open(redir->out_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		print_error_openining_file(redir->out_name);
		return (1);
	}
	close(fd);
	return (0);
}
