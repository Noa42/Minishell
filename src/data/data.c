#include "../../include/minishell.h"

void	init_data(t_data *data, char **env)
{
	data->env = copy_alloc_array(env);
	data->array_var = copy_alloc_array(env);
	if (data->env == NULL || data->array_var == NULL)
	{
		ft_putstr_fd("Cannot allocate memory\n", 2);
		exit_process(data, 1);
	}
	sort_strings(data->array_var, array_len(data->array_var));
	data->input = NULL;
	data->cmd_list = NULL;
	data->exit_status = 0;
	data->token_list = NULL;
	data->here_doc_counter = 0;
	data->parsing_error = 0;
	//data-> pipe = NULL;
}

void	free_data(t_data *data)
{
	//Está por rellenar
	if (data->env)
		free_array(data->env);
	if (data->array_var)
		free_array(data->array_var);
	if (data->cmd_list)
		free_cmd_list(data->cmd_list);
	if (data->token_list)
		free_token_list(data->token_list);
	if (data->input)
		free(data->input);
	rl_clear_history();//limpia el historial de readline
	close_fds();
}
void	reboot_data(t_data *data)
{
	//Rellenar con el la memoria que haya alocado alvaro
	if (data->input)
		free(data->input);
	if (data->cmd_list)
		data->cmd_list = free_cmd_list(data->cmd_list);
	if (data->token_list)
		data->token_list = free_token_list(data->token_list);
	data->input = NULL;
	data->cmd_list = NULL;
	data->token_list = NULL;
	data->here_doc_counter = 0;
	data->parsing_error = 0;
	close_fds();
}
