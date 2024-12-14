/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_5.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/07 20:16:22 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/13 14:55:54 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// es un pequeño parche del parsing:
// esta función comprueba si a la array de strings ha llegado algún carácter
// especial que no debería haber llegado. Por ejemplo,
// en el input cat file.txt "|" grep 2  el carácter especial
// llegará como |   (simplemente, sin comillas). Sin embargo, en este caso,
// sí deberá guardarse
// en la string con comillas.

void	ft_check_if_csp(char *str, t_parsing *prs)
{
	if (prs->arr_lexems[prs->index_arr][0] == '|'
		&& ft_strlen(prs->arr_lexems[prs->index_arr]) == 1 && ft_tell_if_oq(str,
			prs->init_index - 2, 0, 0) == 1 && prs->init_index > 0)
	{
		ft_realloc_prs_o(prs);
		prs->arr_lexems[prs->index_arr][1] = '|';
	}
	if (prs->arr_lexems[prs->index_arr][0] == '<'
		&& ft_strlen(prs->arr_lexems[prs->index_arr]) == 1 && ft_tell_if_oq(str,
			prs->init_index - 2, 0, 0) == 1)
	{
		ft_realloc_prs_o(prs);
		prs->arr_lexems[prs->index_arr][1] = '<';
	}
	if (prs->arr_lexems[prs->index_arr][0] == '<'
		&& prs->arr_lexems[prs->index_arr][1] == '<'
		&& ft_strlen(prs->arr_lexems[prs->index_arr]) == 2 && ft_tell_if_oq(str,
			prs->init_index - 3, 0, 0) == 1)
	{
		ft_realloc_prs_t(prs);
		prs->arr_lexems[prs->index_arr][1] = '<';
		prs->arr_lexems[prs->index_arr][2] = '<';
	}
	else
		ft_check_if_csp_aux(str, prs);
}

void	ft_check_if_csp_aux(char *str, t_parsing *prs)
{
	if (prs->arr_lexems[prs->index_arr][0] == '>'
		&& prs->arr_lexems[prs->index_arr][1] == '>'
		&& ft_strlen(prs->arr_lexems[prs->index_arr]) == 2 && ft_tell_if_oq(str,
			prs->init_index - 3, 0, 0) == 1)
	{
		ft_realloc_prs_t(prs);
		prs->arr_lexems[prs->index_arr][1] = '>';
		prs->arr_lexems[prs->index_arr][2] = '>';
	}
	if (prs->arr_lexems[prs->index_arr][0] == '>'
		&& ft_strlen(prs->arr_lexems[prs->index_arr]) == 1 && ft_tell_if_oq(str,
			prs->init_index - 2, 0, 0) == 1)
	{
		ft_realloc_prs_o(prs);
		prs->arr_lexems[prs->index_arr][1] = '>';
	}
}

void	ft_realloc_prs_o(t_parsing *prs)
{
	free(prs->arr_lexems[prs->index_arr]);
	prs->arr_lexems[prs->index_arr] = NULL;
	prs->arr_lexems[prs->index_arr] = malloc(sizeof(char) * 4);
	if (!prs->arr_lexems[prs->index_arr])
	{
		printf("String has not been created (Error).");
		exit(0);
	}
	prs->arr_lexems[prs->index_arr][0] = '"';
	prs->arr_lexems[prs->index_arr][2] = '"';
	prs->arr_lexems[prs->index_arr][3] = '\0';
}

void	ft_realloc_prs_t(t_parsing *prs)
{
	free(prs->arr_lexems[prs->index_arr]);
	prs->arr_lexems[prs->index_arr] = NULL;
	prs->arr_lexems[prs->index_arr] = malloc(sizeof(char) * 5);
	if (!prs->arr_lexems[prs->index_arr])
	{
		printf("String has not been created (Error).");
		exit(0);
	}
	prs->arr_lexems[prs->index_arr][0] = '"';
	prs->arr_lexems[prs->index_arr][3] = '"';
	prs->arr_lexems[prs->index_arr][4] = '\0';
}
