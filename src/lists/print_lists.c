/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_lists.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:54 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 11:40:49 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*p;

	p = cmd_list;
	printf("+++++++++++++++CMD_LIST+++++++++++++++\n");
	if (p == NULL)
	{
		printf("Empty cmd_list\n");
		return ;
	}
	while (p)
	{
		print_cmd(p);
		print_redir_list(p->redir_list);
		p = p->next;
	}
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");
}

void	print_cmd(t_cmd *cmd)
{
	printf("------------[%i]CMD---------------- \n", cmd->index);
	print_array(cmd->array_cmd);
	printf("fd_in: %i\n", cmd->fd_in);
	printf("fd_out: %i\n", cmd->fd_out);
	printf("\n");
}

void	print_redir(t_redir *redir)
{
	if (redir == NULL)
	{
		printf("Empty redir\n");
		return ;
	}
	printf("[%i]Redir:\n", redir->index);
	printf("-Type: %s\n", redir_type_to_string(redir->type));
	printf("-In_name: %s\n", redir->in_name);
	printf("-Out_name: %s\n", redir->out_name);
	printf("-Delim: %s\n", redir->delim);
}

void	print_redir_list(t_redir *redir_list)
{
	t_redir	*p;

	p = redir_list;
	printf("+REDIR_LIST+\n");
	if (!p)
	{
		printf("Empty redir_list\n");
		return ;
	}
	while (p)
	{
		print_redir(p);
		p = p->next;
	}
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");
}

char	*redir_type_to_string(t_redir_type type)
{
	if (type == INPUT)
		return ("INPUT");
	else if (type == OUTPUT)
		return ("OUTPUT");
	else if (type == APPEND)
		return ("APPEND");
	else if (type == HERE_DOC)
		return ("HERE_DOC");
	return ("UNKNOWN");
}
