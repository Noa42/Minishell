#include "../include/minishell.h"

t_token *add_token(t_token *token_list, t_token_type token_type)
{
    t_token *new_token;
	t_token *p;

	new_token = malloc(sizeof(t_token));
    new_token->token_type = token_type;
    new_token->next = NULL;
    // Si la lista está vacía, el nuevo token es el primero
    if (token_list == NULL)
	{
		new_token->index = 0;
        return (new_token);
	}
    // Si la lista ya tiene elementos, recorre hasta el final y añade el nuevo token
    p = token_list;
    while (p->next)
        p = p->next;
    p->next = new_token;
	new_token->index = token_list_len(token_list) - 1;
    return (token_list);
}

int token_list_len(t_token *token_list)
{
	t_token *p;
	int len;

	len = 0;
	p = token_list;
	if (p == NULL)
		return (0);
	while (p)
	{
		len++;
		p = p->next;
	}
	return (len);
}

t_token *free_token_list(t_token *token_list)
{
	t_token *p;
	t_token *tmp;

	p = token_list;
	while (p)
	{
		tmp = p->next;
		free(p);
		p = tmp;
	}
	return (NULL);
	}