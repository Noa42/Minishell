/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:34:14 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/22 11:56:12 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void    ft_check_if_expans_var(t_parsing *prs)
 {
    int cnt_arr;
    int cnt_str;
    int cnt_dollar;

    cnt_arr = 0;
    cnt_str = 0;
    cnt_dollar = 0;
    while(prs->arr_lexems[cnt_arr] != NULL)
    {
        cnt_str = 0;
        while(prs->arr_lexems[cnt_arr][cnt_str] != '\0')
        {
            if(prs->arr_lexems[cnt_arr][cnt_str] == '$')
            {
                cnt_dollar++;
                printf("FT_CHECK_DOLLAR%s\n", prs->arr_lexems[cnt_arr]);
                prs->flag = 0;
                if (ft_check_dollar(prs, cnt_dollar) == 0)
                {
                    printf("FT_CHECK_DOLLAR%s\n", prs->arr_lexems[cnt_arr]);
                    //prs->arr_lexems[cnt_arr] = expand_vars_hd(prs->arr_lexems[cnt_arr], prs->ptrdata->env);
                    //printf("2   2    LLEGAS?????%s\n", prs->arr_lexems[cnt_arr]);
                }
            }
            cnt_str++;
        }
        cnt_arr++;
    }
}


//si esta función devuelve 0 SÍ expandimos, de lo contrario no!
int     ft_check_dollar(t_parsing *prs, int index)
{
    int count;
    int cnt_dol;

    cnt_dol = 0;
    count = 0;
    while(prs->ptrdata->in_ax[count] != '\0')
    {
	    if (prs->ptrdata->in_ax[count] == DOUB_QUOT)
			ft_if_doub_quote_exp(prs);
		else if (prs->ptrdata->in_ax[count] == ONE_QUOT)
			ft_if_single_quote_exp(prs);
        if (prs->ptrdata->in_ax[count] == '$')
            cnt_dol++;
        if((cnt_dol == index && prs->flag == 0) || (cnt_dol == index && prs->flag == DOUB_QUOT))
            return (0);
        count++;
    }
    return (1);
}

//si está función devuelve 0 SÍ expandimos, de lo contrario no!




int     ft_tell_me_where(char   *str, char c)
{
	int		i;
	
    if (str == NULL)
     return (-1);
    i = 0;
    while (str[i] != c && str[i] != '\0')
        i++;
    return (i);
}

void	ft_if_doub_quote_exp(t_parsing *prs)
{
	if (prs->flag == 0)
		prs->flag = DOUB_QUOT;
	else if (prs->flag == DOUB_QUOT)
		prs->flag = 0;
}


void	ft_if_single_quote_exp(t_parsing *prs)
{
	if (prs->flag == 0)
		prs->flag = ONE_QUOT;
	else if (prs->flag == ONE_QUOT)
		prs->flag = 0;
}

/**
void    ft_check_if_expans_var(t_parsing *prs)
{
    int cnt_arr;
    //int cnt_str;
   int cnt_dollar;

    cnt_arr = 0;
    //cnt_str = 0;
    cnt_dollar = 0;
    while(prs->arr_lexems[cnt_arr] != NULL)
    {
        if (has_variable(prs->arr_lexems[cnt_arr]) != 0) //&& ft(ft_strchr(prs->arr_lexems[cnt_arr], '$')))
        {
            cnt_dollar++;
            if (ft_check_dollar(prs, cnt_dollar) == 0)
                prs->arr_lexems[cnt_arr] = expand_vars_hd(prs->arr_lexems[cnt_arr], prs->ptrdata->env);
            cnt_dollar = cnt_dollar + has_variable(prs->arr_lexems[cnt_arr]) - 1;
        }
        cnt_arr ++;
    }
}*/




/*es una extensión de la función how_mux_lem:  cuando llegamos a las comillas,
	si estamos en un estado libre (flag = 0) abrimos estado,
	si estamos en estado abierto, lo cerramos,
	si el estado está abierto pero por otro tipo de comillas no haremos nada,
	es decir, trataremos a esas comillas como un carácter más*/
    