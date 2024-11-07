#include "../include/minishell.h"

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

char *token_type_to_string(t_token_type token_type)
{
	if (token_type == CMD)
		return ("CMD");
	else if (token_type == PIPE)
		return ("PIPE");
	else if (token_type == REDIR)
		return ("REDIR");
	return ("UNKNOWN");
}
void print_token_list(t_token *token_list)
{
    t_token *p;
	
	p = token_list;
	printf("+++++++++++++++TOKEN_LIST+++++++++++++++\n");
    if (!p)
    {
        printf("Empty token_list\n");
        return;
    }
    while (p)
    {
        printf("[%i]Token: %s\n",p->index, token_type_to_string(p->token_type));
        p = p->next;
    }
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");
}
int cmd_list_len(t_cmd *cmd_list)
{
	t_cmd *p;
	int len;

	len = 0;
	p = cmd_list;
	if (p == NULL)
		return (0);
	while (p)
	{
		len++;
		p = p->next;
	}
	return (len);
}
t_cmd *new_cmd(char **array_cmds, t_data *data)
{
	t_cmd *new_cmd;

	new_cmd = malloc(sizeof(t_cmd));
	new_cmd->array_cmd = array_cmds;
	new_cmd->fd_in = STDIN_FILENO;
	new_cmd->fd_out = STDOUT_FILENO;
	new_cmd->redir_list = NULL;
	new_cmd->data = data;
	new_cmd->next = NULL;
	return (new_cmd);
}
t_cmd *add_cmd(t_cmd *cmd_list, t_cmd *new_cmd)
{
	t_cmd *p;

	if (cmd_list == NULL)
	{
		new_cmd->index = 0;
		return (new_cmd);
	}
	p = cmd_list;
	while (p->next)
		p = p->next;
	p->next = new_cmd;
	new_cmd->index = cmd_list_len(cmd_list)- 1;
	return (cmd_list);
}
void print_cmd(t_cmd *cmd)
{
	printf("------------[%i]CMD---------------- \n", cmd->index);
	print_array(cmd->array_cmd);
	printf("fd_in: %i\n", cmd->fd_in);
	printf("fd_out: %i\n", cmd->fd_out);
	printf("\n");
}

void print_cmd_list(t_cmd *cmd_list)
{
	t_cmd *p;

	p = cmd_list;
	printf("+++++++++++++++CMD_LIST+++++++++++++++\n");
	if (!p)
	{
		printf("Empty cmd_list\n");
		return;
	}
	while (p)
	{
		print_cmd(p);
		print_redir_list(p->redir_list);
		p = p->next;
	}
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");
}
void free_cmd(t_cmd *cmd)
{
	if (cmd->array_cmd)
		free_array(cmd->array_cmd);
	if (cmd->redir_list)
		free_redir_list(cmd->redir_list);
	free(cmd);
	cmd = NULL;
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

t_cmd *free_cmd_list(t_cmd *cmd_list)
{
	t_cmd *p;
	t_cmd *tmp;

	p = cmd_list;
	while (p)
	{
		tmp = p->next;
		if (p->redir_list)
			p->redir_list = free_redir_list(p->redir_list);
		free_cmd(p);
		p = tmp;
	}
	return (NULL);
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
t_cmd *get_last_cmd(t_cmd *cmd_list)
{
	t_cmd *p;

	p = cmd_list;
	if (p == NULL)
		return (NULL);
	while (p->next)
		p = p->next;
	return (p);
}

t_cmd *get_cmd_by_index(t_cmd *cmd_list, int index)
{
	t_cmd *p;

	p = cmd_list;
	if (p == NULL)
		return (NULL);
	while (p)
	{
		if (p->index == index)
			return (p);
		p = p->next;
	}
	return (NULL);
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
}
t_redir *new_redir(t_redir_type type, char *in_name, char *out_name)
{
	t_redir *new_redir;

	new_redir = malloc(sizeof(t_redir));
	new_redir->in_name = in_name;
	new_redir->out_name = out_name;
	new_redir->type = type;
	new_redir->fd_in = STDIN_FILENO;
	new_redir->fd_out = STDOUT_FILENO;
	new_redir->next = NULL;
	new_redir->data = NULL;
	return (new_redir);
}
char *redir_type_to_string(t_redir_type type)
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
void print_redir_list(t_redir *redir_list)
{
	t_redir *p;
	int i;

	i = 0;
	p = redir_list;
	printf("+REDIR_LIST+\n");
	if (!p)
	{
		printf("Empty redir_list\n");
		return;
	}
	while (p)
	{	
		printf("[%i]Redir:\n", i);
		printf("Type: %s\n", redir_type_to_string(p->type));
		printf("In_name: %s\n", p->in_name);
		printf("Out_name: %s\n", p->out_name);
		printf("fd_in: %i\n", p->fd_in);
		printf("fd_out: %i\n", p->fd_out);
		i++;
		p = p->next;
	}
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");
}

void instrucciones_ejemplo_listas(t_data *data)
{
	//INSTRUCCIONES DE COMO USAR LAS LISTAS

	//LISTA DE TOKENS
	////Para añadir tokens igualamos la data->token_list a la función add_token y le pasamos el tipo de token que queremos añadir
	data->token_list = add_token(data->token_list, CMD);
	data->token_list = add_token(data->token_list, PIPE);
	data->token_list = add_token(data->token_list, REDIR);
	////Para imprimir tenemos print_token_list
	print_token_list(data->token_list);
	////Para liberar la memoria de la lista de tokens igualamos data->token_list a la función free_token_list
	data->token_list = free_token_list(data->token_list);

	//LISTA DE COMANDOS
	//Para añadir comandos hay dos opciones:

	//1ºOPCION.
	////1.-. Crear un cmd* con new_cmd, que recibe un array de strings con el comando y sus argumentos, lo podemos modificar como queramos accediendo a sus campos
	////NUEVO!!!! -> Ahora new_cmd recibe también la data para enlazar su cmd->data con la data que le pasamos
	t_cmd *cmd = new_cmd(ft_split("ls -l", ' '), data);
	////Igualamos data->cmd_list a la función add_cmd, que recibe la lista de comandos y el comando que queremos añadir
	data->cmd_list = add_cmd(data->cmd_list, cmd);

	//2ºOPCION.
	////Hacerlo todo a la vez del tirón, es decir igualar y usar el new_cmd en la misma línea
	////NUEVO!!!! -> Ahora new_cmd recibe también la data para enlazar su cmd->data con la data que le pasamos
	data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split("ls -l", ' '), data));

	//Modificar un comando de la lista. Para esto tenemos dos funciones
	//GET_CMD_BY_INDEX: Para modificar un comando concreto de la lista usamos get_cmd_by_index, que recibe la lista de comandos y el índice del comando que queremos modificar
	t_cmd *cmd_to_modify = get_cmd_by_index(data->cmd_list, 0);
	//Ahora podemos modificar el comando como queramos
	cmd_to_modify->fd_in = 3;
	cmd_to_modify->fd_out = 4;
	//GET_LAST_CMD: Para modificar el último comando de la lista usamos get_last_cmd, que recibe la lista de comandos
	t_cmd *last_cmd = get_last_cmd(data->cmd_list);
	//Ahora podemos modificar el comando como queramos
	last_cmd->fd_in = 5;
	last_cmd->fd_out = 6;

	////REDIRECCIONES
	//A un comando concreto (ya se el último o el que queramos obtener con el índice) le añadimos redirecciones con la función add_redir
	add_redir(get_last_cmd(data->cmd_list), new_redir(HERE_DOC, "in3.txt", "out3.txt"));//al último le añadimos una primera redirección de tipo HERE_DOC
	add_redir(get_last_cmd(data->cmd_list), new_redir(OUTPUT, "in4.txt", "out4.txt")); //al último le añadimos una segunda redirección de tipo OUTPUT
	add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in1.txt", "out1.txt"));//al comando con índice 0 le añadimos una redirección de tipo INPUT
	add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "in2.txt", "out2.txt"));//al comando con índice 0 le añadimos una redirección de tipo APPEND


	//Para imprimir la lista de comandos usamos print_cmd_list (te imprime tambien las redirecciones si las hay)
	print_cmd_list(data->cmd_list);

	//Para liberar la memoria de las listas usamos free_cmd_list igualandola a las lista original
	data->cmd_list = free_cmd_list(data->cmd_list);

	//Esto es para comprobar que están vacías
	print_token_list(data->token_list);
	print_cmd_list(data->cmd_list);

}
/////INSTRUCCIONES/////
/*
	//Instrucciones para trabajar con la lista de comandos
	//Para añadir comandos a la lista:
	data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split("ls -l", ' '))); //esto solo llena el campo de array_cmds, el index y los fds. Las redirecciones se quedan vacías
	//Repetimos los pasos para añadir otro comando
	data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split("cat", ' ')));
	add_redir(get_last_cmd(data->cmd_list), new_redir(HERE_DOC, "in3.txt", "out3.txt"));
	add_redir(get_last_cmd(data->cmd_list), new_redir(OUTPUT, "in4.txt", "out4.txt"));	
	add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in1.txt", "out1.txt"));
	add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "in2.txt", "out2.txt"));	
	print_cmd_list(data->cmd_list);
	printf("Despues de liberar:\n");
	data->cmd_list = free_cmd_list(data->cmd_list);
	print_cmd_list(data->cmd_list);
*/