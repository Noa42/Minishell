/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:34:14 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/21 14:41:19 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// void    ft_check_if_expans_var(t_parsing *prs)
// {
//     int cnt_arr;
//     int cnt_str;
//     int cnt_dollar;

//     cnt_arr = 0;
//     cnt_str = 0;
//     cnt_dollar = 0;
//     while(prs->arr_lexems[cnt_arr] != NULL)
//     {
//         while(prs->arr_lexems[cnt_arr][cnt_str] != '\0')
//         {
//             printf("LLEGAS?????%s\n", prs->arr_lexems[cnt_arr]);
//             if(prs->arr_lexems[cnt_arr][cnt_str] == '$')
//             {
//                 cnt_dollar++;
//                 if (ft_check_dollar(prs, cnt_str) == 0)
//                 {
//                     printf("LLEGAS?????%s\n", prs->arr_lexems[cnt_arr]);
//                     prs->arr_lexems[cnt_arr] = expand_vars_hd(prs->arr_lexems[cnt_arr], prs->ptrdata->env);
//                     printf("2   2    LLEGAS?????%s\n", prs->arr_lexems[cnt_arr]);
//                 }
//             }
//             cnt_str++;
//         }
//         cnt_arr++;
//     }
// }

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
}

int    ft_check_dollar(t_parsing *prs, int index)
{
    int count;
    int cnt_dol;

    cnt_dol = 0;
    count = 0;
    while(prs->ptrdata->in_ax[count] != '\0')
    {
        if(prs->ptrdata->in_ax[count] == '$')
        {
            cnt_dol++;
            if(cnt_dol == index)
                return (ft_tell_if_dllr_qt(prs, count));
        }
        count++;
    }
    return (1);
}

//si está función devuelve 0 SÍ expandimos, de lo contrario no!
int ft_tell_if_dllr_qt(t_parsing *prs, int index)
{
    int count;
    int flag;

    flag = 0;
    count = 0;
    while(prs->ptrdata->in_ax[count] != '\0' && count < index)
    {
	    if (prs->ptrdata->in_ax[count] == ONE_QUOT)
	    {
		    if (flag == 0)
			    flag = ONE_QUOT;
		    else if (flag == ONE_QUOT)
			    flag = 0;
	    }
	    count++;
    }
	if ((prs->ptrdata->in_ax[count]) && flag == 0)
	    return (0);
    return (1);
}




/*
int	ft_tell_if_oq(char *str, int index, int i, int flag)
{
	if (index < 0)
		return (0);
	while (str[i] != '\0' && i <= index)
	{
		if (str[i] == DOUB_QUOT)
		{
			if (flag == 0)
				flag = DOUB_QUOT;
			else if (flag == DOUB_QUOT)
				flag = 0;
		}
		if (str[i] == ONE_QUOT)
		{
			if (flag == 0)
				flag = ONE_QUOT;
			else if (flag == ONE_QUOT)
				flag = 0;
		}
		if ((i == index) && flag == 0)
			return (0);
		i++;
	}
	return (1);
}*/