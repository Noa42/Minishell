/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sending_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:33:21 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/19 01:22:00 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Aquí vamos a empezar a usar las funciones de listas, así que conviene
// que esta función tenda prs y data, ya que las funciones de listas que
// usamos usan data y necesitan nutrirse de una array de lexemas que está
// en prs

void	ft_start_sending(t_parsing *prs, t_data *data)
{
	t_cmd	*node;
	int		i;
	int		flag;

	flag = 0;
	node = NULL;
	i = 0;
	while (prs->arr_lexems[i] != NULL && flag == 0)
	{
		if ((ft_strlen(prs->arr_lexems[i]) == 1 && prs->arr_lexems[i][0] == '<')
			|| (ft_strlen(prs->arr_lexems[i]) == 1
				&& prs->arr_lexems[i][0] == '>')
			|| (ft_strlen(prs->arr_lexems[i]) == 2
				&& prs->arr_lexems[i][0] == '<' && prs->arr_lexems[i][1] == '<')
			|| (ft_strlen(prs->arr_lexems[i]) == 2
				&& prs->arr_lexems[i][0] == '>' && prs->arr_lexems[i][1] == '>')
			|| (ft_strlen(prs->arr_lexems[i]) == 1
				&& prs->arr_lexems[i][0] == '|'))
		{
			ft_if_pipe_or_rdr(prs, data, node);
			flag = 1;
		}
		i++;
	}
	if (flag == 0)
		ft_if_not_special_char(prs, data, node);
}

void	ft_if_not_special_char(t_parsing *prs, t_data *data, t_cmd *node)
{
	char	**array;

	array = copy_alloc_array(prs->arr_lexems);
	node = new_cmd(array, data);
	data->cmd_list = add_cmd(data->cmd_list, node);
	print_cmd_list(data->cmd_list);
}

void	ft_if_pipe_or_rdr(t_parsing *prs, t_data *data, t_cmd *node)
{
	int	pipes;

	pipes = ft_tell_me_if_pipes(prs);
	if (pipes > 0)
	{
        ft_send_if_pipe(prs, pipes, 0);
		return ;
	}
	ft_send_to_cmd_str(prs, data, node, 0);
}

void	ft_send_to_cmd_str(t_parsing *prs, t_data *data, t_cmd *node, int i)
{
	prs->aux_ar_cmds = filter_strings(prs->arr_lexems);
	prs->aux_redirs = filter_redirections(prs->arr_lexems);
	node = new_cmd(copy_alloc_array(prs->aux_ar_cmds), data);
	data->cmd_list = add_cmd(data->cmd_list, node);
	while (prs->aux_redirs[i] != NULL)
	{
		ft_send_rd(prs, data, i);
		i++;
	}
	// print_cmd_list(data->cmd_list);
}

int	ft_tell_me_if_pipes(t_parsing *prs)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (prs->arr_lexems && prs->arr_lexems[i] != NULL)
	{
		if (prs->arr_lexems[i] && ft_strlen(prs->arr_lexems[i]) == 1
			&& prs->arr_lexems[i][0] == '|')
			count++;
		i++;
	}
	return (count);
}