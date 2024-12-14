/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 13:31:03 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/13 14:55:36 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

/*esta función crea una array de strings donde estarán los lexemas*/
void	ft_create_arr_lexem(char *str, t_parsing *prs)
{
	prs->arr_lexems = malloc(sizeof(char *) * (prs->how_much + 1));
	if (!prs->arr_lexems)
	{
		printf("The array has not been created (Error).");
		exit(0);
	}
	prs->arr_lexems[prs->how_much] = 0;
	prs->count = 0;
	prs->flag = 0;
	prs->reject = 0;
	ft_string_by_string(str, prs);
	//ft_create_tk_arr(prs);
}

/*esta función va iterando el data.input para ir diciendo dónde hay que
"splitear"*/
void	ft_string_by_string(char *str, t_parsing *prs)
{
	while (str[prs->count] != '\0' && prs->index_arr < prs->how_much)
		ft_string_by_string_aux(str, prs);
	prs->arr_lexems[prs->how_much] = 0;
}

void	ft_string_by_string_aux(char *str, t_parsing *prs)
{
	while (prs->flag == 0 && (str[prs->count] == 32 || str[prs->count] == '\t'))
	{
		prs->count++;
		prs->init_index = prs->count;
	}
	if (str[prs->count] == DOUB_QUOT)
		ft_if_doub_quote(prs);
	if (str[prs->count] == ONE_QUOT)
		ft_if_single_quote(prs);
	if ((str[prs->count + 1] == 32 || str[prs->count + 1] == '\t'
			|| str[prs->count + 1] == '\0') && prs->flag == 0)
	{
		prs->end_index = prs->count;
		if (ft_if_lexem_only_doub_quotes(str, prs) == 1
			|| ft_if_lexem_only_single_quotes(str, prs) == 1)
			ft_create_a_space_lexem(prs);
		else
			ft_split_strings(str, prs);
		prs->index_arr++;
		prs->reject = 0;
	}
	prs->count++;
}

/*esta función va spliteando el data.input y va completando, string a
string, la array de strings que habáimos inicialiado previamente*/
void	ft_split_strings(char *str, t_parsing *prs)
{
	prs->cnt_new_str = 0;
	prs->len = (prs->end_index - prs->init_index - prs->reject) + 2;
	prs->arr_lexems[prs->index_arr] = malloc(sizeof(char) * prs->len);
	if (!prs->arr_lexems[prs->index_arr])
	{
		printf("String has not been created (Error)");
		exit(0);
	}
	while (prs->init_index <= prs->end_index)
	{
		if (str[prs->init_index] == DOUB_QUOT)
			ft_if_doub_quote_split_strings(prs, str);
		if (str[prs->init_index] == ONE_QUOT)
			ft_if_single_quote_split_strings(prs, str);
		if (str[prs->init_index] != ONE_QUOT
			&& str[prs->init_index] != DOUB_QUOT)
		{
			prs->arr_lexems[prs->index_arr][prs->cnt_new_str] \
			= str[prs->init_index];
			prs->cnt_new_str++;
		}
		prs->init_index++;
	}
	prs->arr_lexems[prs->index_arr][prs->cnt_new_str] = '\0';
	ft_check_if_csp(str, prs);
}

/*esta función es una extensión de ft_split_strings: si es una comilla de
apertura o de cierre no copia, en cualquier otro caso, sí copia*/
void	ft_if_doub_quote_split_strings(t_parsing *prs, char *str)
{
	if (prs->flag == 0)
		prs->flag = DOUB_QUOT;
	else if (prs->flag == DOUB_QUOT)
		prs->flag = 0;
	if (prs->flag == ONE_QUOT)
	{
		prs->arr_lexems[prs->index_arr][prs->cnt_new_str] \
		= str[prs->init_index];
		prs->cnt_new_str++;
	}
}
