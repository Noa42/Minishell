#include "../include/minishell.h"

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
void print_cmd(t_cmd *cmd)
{
	printf("------------[%i]CMD---------------- \n", cmd->index);
	print_array(cmd->array_cmd);
	printf("fd_in: %i\n", cmd->fd_in);
	printf("fd_out: %i\n", cmd->fd_out);
	printf("\n");
}

void print_redir(t_redir *redir)
{
	if(redir==NULL)
	{
		printf("Empty redir\n");
		return;
	}
	printf("[%i]Redir:\n", redir->index);
	printf("-Type: %s\n", redir_type_to_string(redir->type));
	printf("-In_name: %s\n", redir->in_name);
	printf("-Out_name: %s\n", redir->out_name);
	printf("-Delim: %s\n", redir->delim);
	printf("-fd_in: %i\n", redir->fd_in);
	printf("-fd_out: %i\n", redir->fd_out);
}

void print_redir_list(t_redir *redir_list)
{
	t_redir *p;

	p = redir_list;
	printf("+REDIR_LIST+\n");
	if (!p)
	{
		printf("Empty redir_list\n");
		return;
	}
	while (p)
	{	
		print_redir(p);
		p = p->next;
	}
	printf("+++++++++++++++++++++++++++++++++++++++\n\n");
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
	//NUEVO ahora al hacer new_redir le pasamos el tipo y el nombre del archivo (la función se encarga de guardarala en in_name o out_name en función del tipo) Si es Heredoc lo se le megte es el delimitador o end_marker
	add_redir(get_last_cmd(data->cmd_list), new_redir(HERE_DOC, NULL));//al último le añadimos una primera redirección de tipo HERE_DOC
	add_redir(get_last_cmd(data->cmd_list), new_redir(OUTPUT, "out4.txt")); //al último le añadimos una segunda redirección de tipo OUTPUT
	add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in1.txt"));//al comando con índice 0 le añadimos una redirección de tipo INPUT
	add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "out.txt"));//al comando con índice 0 le añadimos una redirección de tipo APPEND


	//Para imprimir la lista de comandos usamos print_cmd_list (te imprime tambien las redirecciones si las hay)
	print_cmd_list(data->cmd_list);

	//Para liberar la memoria de las listas usamos free_cmd_list igualandola a las lista original
	data->cmd_list = free_cmd_list(data->cmd_list);

	//Esto es para comprobar que están vacías
	print_token_list(data->token_list);
	print_cmd_list(data->cmd_list);

}