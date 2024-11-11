#include "../include/minishell.h"

t_redir *new_redir(t_redir_type type, char *input) //la input es siempre un char * que dependiendo del tipo de redirecion se guarda como nombre del archivo de salida o de entrada o como el delimitador del heredoc
{
	t_redir *new_redir;

	new_redir = malloc(sizeof(t_redir));
	new_redir->type = type;
	new_redir->in_name = NULL;
	new_redir->out_name = NULL;
	new_redir->delim = NULL;
	if (type == INPUT)
		new_redir->in_name = input;
	else if (type == OUTPUT || type == APPEND)
		new_redir->out_name = input;
	else if (type == HERE_DOC)
		new_redir->delim = input;
	new_redir->fd_in = STDIN_FILENO;
	new_redir->fd_out = STDOUT_FILENO;
	new_redir->data = NULL;
	new_redir->next = NULL;
	return (new_redir);
}

void update_index_redir_list(t_redir *redir_list)
{
	t_redir *p;
	int i;

	i = 0;
	p = redir_list;
	if (p == NULL)
		return ;
	while (p)
	{
		p->index = i;
		i++;
		p = p->next;
	}
}
void add_redir(t_cmd *cmd, t_redir *redir)
{
	t_redir *p;

	if (cmd->redir_list == NULL)
	{
		cmd->redir_list = redir;
		return ;
	}
	p = cmd->redir_list;
	while (p->next)
		p = p->next;
	p->next = redir;
	update_index_redir_list(cmd->redir_list);
}
t_redir *free_redir_list(t_redir *redir_list)
{
	t_redir *p;
	t_redir *tmp;

	p = redir_list;
	while (p)
	{
		tmp = p->next;
		free(p);
		p = tmp;
	}
	return (NULL);
}