/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 00:02:28 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/13 14:55:29 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// Esta función inicializa al estructura que vamos a necesitar para el parseo

void	ft_init_parsing_struc(t_parsing *prs)
{
	prs->count = 0;
	prs->how_much = 0;
	prs->flag = 0;
	prs->reject = 0;
	prs->arr_lexems = NULL;
	prs->arr_tokens = NULL;
	prs->init_index = 0;
	prs->end_index = 0;
	prs->index_arr = 0;
	prs->cnt_new_str = 0;
	prs->len = 0;
	prs->flag_space = 'g';
}

// esta función comprueba si el input está compuesto solo por espacios, en tal
// caso volveremos a reboot data, también pone el tope en el último carácter
// válido del input (cualquier carácter menos un espacio).
void	ft_parsing(t_data *data)
{
	int		top;
	int		new_len;

	new_len = ft_len(data->input, 0, 0, 0) + ft_strlen(data->input);
	data->in_ax = ft_new_input(data->input, new_len);
	top = ft_strlen(data->in_ax) - 1;
	if (ft_only_spaces(data->in_ax) == 1)
		ft_print_prs_err('s', &data->prs);
	else
	{
		while (data->in_ax[top] == ' ')
			top--;
		ft_start_parsing(data->in_ax, &data->prs, top);
	}
}

/*Esta función cuenta cuántos lexemas tiene la entrada y, además, cuenta
	cuántos carácteres nos son prescindibles: nos son prescindibles las
	comillas que cierren y que abren estados, y los espacios entre lexemas
	o los espacios antes del primer lexema. Al saber los carácteres
	prescindibles, podremos alocar la memoria correcta posteriormente.*/
void	ft_start_parsing(char *str, t_parsing *prs, int top)
{
	while (str[prs->count] != '\0' && prs->count <= top)
	{
		if (prs->flag == 0 && (str[prs->count] == 32
				|| str[prs->count] == '\t'))
			prs->count++;
		while (prs->flag == 0 && (str[prs->count] == 32
				|| str[prs->count] == '\t'))
		{
			prs->count++;
			prs->reject++;
		}
		if (str[prs->count] == DOUB_QUOT)
			ft_if_doub_quote(prs);
		if (str[prs->count] == ONE_QUOT)
			ft_if_single_quote(prs);
		if ((str[prs->count + 1] == 32 || str[prs->count + 1] == '\t'
				|| str[prs->count + 1] == '\0') && prs->flag == 0)
			prs->how_much++;
		prs->count++;
	}
	if (prs->flag != 0)
		ft_print_prs_err('u', prs);
	else
		ft_create_arr_lexem(prs->ptrdata->in_ax, prs);	
}

/*es una extensión de la función how_mux_lem:  cuando llegamos a las comillas,
   si estamos en un estado libre (flag = 0) abrimos estado,
   si estamos en estado abierto, lo cerramos,
   si el estado está abierto pero por otro tipo de comillas no haremos nada,
   es decir, trataremos a esas comillas como un carácter más*/
void	ft_if_doub_quote(t_parsing *prs)
{
	if (prs->flag == 0)
	{
		prs->flag = DOUB_QUOT;
		prs->reject++;
	}
	else if (prs->flag == DOUB_QUOT)
	{
		prs->flag = 0;
		prs->reject++;
	}
}

/*es una extensión de la función how_mux_lem:  cuando llegamos a las comillas,
	si estamos en un estado libre (flag = 0) abrimos estado,
	si estamos en estado abierto, lo cerramos,
	si el estado está abierto pero por otro tipo de comillas no haremos nada,
	es decir, trataremos a esas comillas como un carácter más*/
void	ft_if_single_quote(t_parsing *prs)
{
	if (prs->flag == 0)
	{
		prs->flag = ONE_QUOT;
		prs->reject++;
	}
	else if (prs->flag == ONE_QUOT)
	{
		prs->flag = 0;
		prs->reject++;
	}
}
