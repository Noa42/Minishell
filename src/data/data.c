/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:41:04 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 12:59:58 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	init_data(t_data *data, char **env)
{
	if (env[0] == NULL)
		empty_env(data);
	else
	{
		data->env = copy_alloc_array(env);
		data->array_var = copy_alloc_array(env);
	}
	if (data->env == NULL || data->array_var == NULL)
	{
		ft_putstr_fd("MiniShell: Cannot allocate memory\n", 2);
		exit_process(data, 1);
	}
	sort_strings(data->array_var, array_len(data->array_var));
	data->input = NULL;
	data->in_ax = NULL;
	data->cmd_list = NULL;
	data->exit_status = 0;
	data->here_doc_counter = 0;
	data->parsing_error = 0;
	data->exit_status = 0;
	data->prs.ptrdata = data;
	get_data(data);
	ft_init_parsing_struc(&data->prs);
	update_shlvl(data);
}

void	free_data(t_data *data)
{
	if (data->env)
		free_array(data->env);
	if (data->array_var)
		free_array(data->array_var);
	if (data->cmd_list)
		free_cmd_list(data->cmd_list);
	if (data->input)
		free(data->input);
	if (data->in_ax)
		free(data->in_ax);
	free_parsing(&data->prs);
	rl_clear_history();
	close_fds();
}

void	reboot_data(t_data *data)
{
	if (data->input)
		free(data->input);
	if (data->cmd_list)
		data->cmd_list = free_cmd_list(data->cmd_list);
	if (data->in_ax)
		free(data->in_ax);
	data->in_ax = NULL;
	data->input = NULL;
	data->cmd_list = NULL;
	data->here_doc_counter = 0;
	data->parsing_error = 0;
	g_signal_flag = 0;
	free_parsing(&data->prs);
	ft_init_parsing_struc(&data->prs);
	signals_handler();
	close_fds();
}

void	free_parsing(t_parsing *prs)
{
	if (prs->arr_lexems)
		free_array(prs->arr_lexems);
	if (prs->arr_toks)
		free(prs->arr_toks);
	if (prs->aux_ar_cmds)
		free_array(prs->aux_ar_cmds);
	if (prs->aux_redirs)
		free_array(prs->aux_redirs);
	free_triple_ptr(prs->ar_of_ar);
	prs->arr_lexems = NULL;
	prs->arr_toks = NULL;
	prs->aux_ar_cmds = NULL;
	prs->aux_redirs = NULL;
	prs->ar_of_ar = NULL;
}
