#include "../include/minishell.h"

int	only_spaces(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] != ' ')
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
	data->exit_status = -1;
	data->token_list = NULL;
	data->parsing_error = 0;
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
    free(data->input);
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
	data->parsing_error = 0;
}

int	main(int argc, char** argv, char **env)
{
	t_data	data;
	argc ++; //esto es para que no me salte un warning de que argc y argv no se usa y el primer argumento siempre tiene que ser de tipo int. Es para coger el env.
	argv[1] = "avoid warning";
	init_data(&data, env);
	signals();
	while (1)
	{
		data.input = readline("MiniShell> ");
		if (data.input == NULL)
			break ;
		history(data.input);
		//parsing(&data);
		//if(data.parsing_error == 0)
		prueba_ejecucion(&data);
		reboot_data(&data);
	}
    free_data(&data);
	printf("Saliendo de MiniShell\n");
	return (0);
}
