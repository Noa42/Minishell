/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sending_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 23:27:49 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/23 00:30:23 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_create_three_ptr(t_parsing *prs, int pipes)
{
	int	count;
	int	init;
	int	end;

	init = 0;
	end = 0;
	count = 0;
	prs->ar_of_ar = malloc(sizeof(char **) * (pipes + 2));
	if (!prs->ar_of_ar)
	{
		ft_putstr_fd("MiniShell: String has not been created.\n", 2);
		exit(0);
	}
	while (prs->arr_lexems[end] != NULL)
	{
		while (prs->arr_lexems[end] != NULL && ft_strcmp(prs->arr_lexems[end],
				"|") != 0)
			end++;
		prs->ar_of_ar[count] = ft_give_me_arr(prs->arr_lexems, init, end);
		init = end + 1;
		if (prs->arr_lexems[end] != NULL)
			end++;
		count++;
	}
	prs->ar_of_ar[count] = NULL;
}

void	ft_send_if_pipe(t_parsing *prs, int pipes, int i)
{
	t_cmd	*node;
	int		count;

	count = 0;
	ft_create_three_ptr(prs, pipes);
	while (prs->ar_of_ar[count] != NULL)
	{
		prs->aux_ar_cmds = filter_strings(prs->ar_of_ar[count]);
		if (prs->aux_ar_cmds == NULL)
			node = new_cmd(copy_alloc_array((char *[]){" ", NULL}),
					prs->ptrdata);
		else
			node = new_cmd(copy_alloc_array(prs->aux_ar_cmds), prs->ptrdata);
		prs->aux_redirs = filter_redirections(prs->ar_of_ar[count]);
		prs->ptrdata->cmd_list = add_cmd(prs->ptrdata->cmd_list, node);
		i = 0;
		while (prs->aux_redirs && prs->aux_redirs[i] != NULL)
			ft_send_rd(prs, prs->ptrdata, i++);
		count++;
		free_array(prs->aux_ar_cmds);
		free_array(prs->aux_redirs);
		prs->aux_ar_cmds = NULL;
		prs->aux_redirs = NULL;
	}
}

void	ft_send_rd(t_parsing *prs, t_data *data, int i)
{
	if (ft_strlen(prs->aux_redirs[i]) == 1 && prs->aux_redirs[i][0] == '<')
	{
		add_redir(get_last_cmd(data->cmd_list), new_redir(INPUT,
				ft_strdup(prs->aux_redirs[i + 1]), data));
	}
	if (ft_strlen(prs->aux_redirs[i]) == 1 && prs->aux_redirs[i][0] == '>')
	{
		add_redir(get_last_cmd(data->cmd_list), new_redir(OUTPUT,
				ft_strdup(prs->aux_redirs[i + 1]), data));
	}
	if (ft_strlen(prs->aux_redirs[i]) == 2 && prs->aux_redirs[i][0] == '<'
		&& prs->aux_redirs[i][1] == '<')
	{
		add_redir(get_last_cmd(data->cmd_list), new_redir(HERE_DOC,
				ft_strdup(prs->aux_redirs[i + 1]), data));
	}
	if (ft_strlen(prs->aux_redirs[i]) == 2 && prs->aux_redirs[i][0] == '>'
		&& prs->aux_redirs[i][1] == '>')
	{
		add_redir(get_last_cmd(data->cmd_list), new_redir(APPEND,
				ft_strdup(prs->aux_redirs[i + 1]), data));
	}
}

int	is_redirection(const char *str)
{
	size_t	len;

	if (!str)
		return (0);
	len = ft_strlen(str);
	return ((len == 1 && (str[0] == '<' || str[0] == '>')) || (len == 2
			&& ((str[0] == '<' && str[1] == '<') || (str[0] == '>'
					&& str[1] == '>'))));
}

int	is_redirection_n(const char *str)
{
	if (!str)
		return (0);
	if (ft_strcmp(str, "<") == 0 || ft_strcmp(str, ">") == 0 || ft_strcmp(str,
			"<<") == 0 || ft_strcmp(str, ">>") == 0)
		return (1);
	return (0);
}
