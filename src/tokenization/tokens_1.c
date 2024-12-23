/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 19:13:18 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/23 01:03:29 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_create_tk_arr(t_parsing *prs)
{
	int	count;
	int	count_2;

	count_2 = 0;
	count = 0;
	while (prs->arr_lexems && prs->arr_lexems[count] != NULL)
		count++;
	prs->arr_toks = malloc(sizeof(int) * (count));
	if (!prs->arr_toks)
	{
		ft_putstr_fd("MiniShell: String has not been created.\n", 2);
		exit(0);
	}
	while (prs->arr_lexems && prs->arr_lexems[count_2] != NULL)
	{
		ft_create_tks(prs, count_2);
		count_2++;
	}
	ft_check_toks(prs, count);
}

void	ft_create_tks(t_parsing *prs, int count_2)
{
	if (ft_strlen(prs->arr_lexems[count_2]) == 1
		&& ft_strcmp(prs->arr_lexems[count_2], "|") == 0)
		prs->arr_toks[count_2] = PIPE;
	else if (ft_strlen(prs->arr_lexems[count_2]) == 1
		&& ft_strcmp(prs->arr_lexems[count_2], "<") == 0)
		prs->arr_toks[count_2] = REDIR;
	else if (ft_strlen(prs->arr_lexems[count_2]) == 1
		&& ft_strcmp(prs->arr_lexems[count_2], ">") == 0)
		prs->arr_toks[count_2] = REDIR;
	else if (ft_strlen(prs->arr_lexems[count_2]) == 2
		&& ft_strcmp(prs->arr_lexems[count_2], "<<") == 0)
		prs->arr_toks[count_2] = REDIR;
	else if (ft_strlen(prs->arr_lexems[count_2]) == 2
		&& ft_strcmp(prs->arr_lexems[count_2], ">>") == 0)
		prs->arr_toks[count_2] = REDIR;
	else
		prs->arr_toks[count_2] = CMD;
}

void	ft_check_toks(t_parsing *prs, int len)
{
	int	count;

	count = 0;
	if (prs->arr_toks[0] == PIPE)
		ft_print_unexpected(prs, 0, 0);
	while (count < len && prs->ptrdata->parsing_error == 0)
	{
		if (prs->arr_toks[count] == REDIR && (count == (len - 1)))
			ft_print_unexpected(prs, count, len);
		else if (prs->arr_toks[count] == PIPE && (count == (len - 1)))
			ft_print_unexpected(prs, count, len);
		else if (prs->arr_toks[count] == REDIR && ((count + 1) < len)
			&& prs->arr_toks[count + 1] == REDIR)
			ft_print_unexpected(prs, count, len);
		else if (prs->arr_toks[count] == PIPE && ((count + 1) < len)
			&& prs->arr_toks[count + 1] == PIPE)
			ft_print_unexpected(prs, count, len);
		else if (prs->arr_toks[count] == REDIR && ((count + 1) < len)
			&& prs->arr_toks[count + 1] == PIPE)
			ft_print_unexpected(prs, count, len);
		count++;
	}
}

void	ft_print_unexpected(t_parsing *prs, int count, int len)
{
	prs->ptrdata->exit_status = 2;
	prs->ptrdata->parsing_error = 1;
	if (prs->arr_toks[0] == PIPE)
		ft_putstr_fd("MiniShell: syntax error near unexpected token `|'\n", 2);
	if (prs->arr_toks[count] == REDIR && (count == (len - 1)))
	{
		ft_putstr_fd("MiniShell: syntax error near unexpected\n", 2);
		ft_putstr_fd(" token `newline'\n", 2);
	}
	else if (prs->arr_toks[count] == PIPE && (count == (len - 1)))
		ft_putstr_fd("MiniShell: syntax error: unexpected end of file\n", 2);
	else if (prs->arr_toks[count] == REDIR && ((count + 1) < len)
		&& prs->arr_toks[count + 1] == REDIR)
		ft_putstr_fd("MiniShell: syntax error near unexpected token\n", 2);
	else if (prs->arr_toks[count] == PIPE && ((count + 1) < len)
		&& prs->arr_toks[count + 1] == PIPE)
		ft_putstr_fd("MiniShell: syntax error near unexpected token\n", 2);
	else if (prs->arr_toks[count] == REDIR && ((count + 1) < len)
		&& prs->arr_toks[count + 1] == PIPE)
		ft_putstr_fd("MiniShell: syntax error near unexpected token `|'\n", 2);
}
