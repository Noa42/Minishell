#include "../include/minishell.h"

int	only_spaces(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

void	history(char *input)
{
	if (input != NULL && ft_strlen(input) != 0 && only_spaces(input) == 0)
		add_history(input);
}

void	init_data(t_data *data, char **env)
{
	data->env = copy_alloc_array(env);
	data->array_var = copy_alloc_array(env);
	sort_strings(data->array_var, array_len(data->array_var));
	data->input = NULL;
	data->cmd_list = NULL;
	data->exit_status = 0;
	data->token_list = NULL;
	data->here_doc_counter = 0;
	//data-> pipe = NULL;
}
void    free_data(t_data *data)
{
	//Está por rellenar
	if(data->env)
		free_array(data->env);
	if(data->array_var)
		free_array(data->array_var);
	if(data->cmd_list)
		free_cmd_list(data->cmd_list);
	if(data->token_list)
		free_token_list(data->token_list);
    if (data->input)
		free(data->input);
	close_fds();
}
void reboot_data(t_data *data)
{
	if(data->input)
		free(data->input);
	if(data->cmd_list)
		data->cmd_list = free_cmd_list(data->cmd_list);
	if(data->token_list)
		data->token_list = free_token_list(data->token_list);
	data->input = NULL;
	data->cmd_list = NULL;
	data->token_list = NULL;
	data->exit_status = 0;
	data->here_doc_counter = 0;
	close_fds();
}

int	interact_mode = 0;

int	main(int argc, char** argv, char **env)
{
	t_data	data;
	int exit_status;

	(void)argc; 
	(void)argv;
	init_data(&data, env);
	signals();
	while (1)
	{
		data.input = readline("MiniShell> ");
		if (data.input == NULL)
			break ;
		history(data.input);
		//parsing(&data);
		if(data.exit_status == 0)
			prueba_ejecucion(&data);
		reboot_data(&data);
	}
	exit_status = data.exit_status; //esto es para que cuando hagamos ctrl+d el exit status sea el que corresponda
    free_data(&data);
	printf("exit\n");
	return (exit_status);
}
