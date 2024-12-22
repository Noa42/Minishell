/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:34:14 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/22 17:19:39 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// si esta función devuelve 0 SÍ expandimos, de lo contrario no!
void	ft_start_expas(t_parsing *prs)
{
	int	count;
    int count_dollar;

    count = 0;
    count_dollar = 0;	
    prs->flag = 0;
	while(prs->ptrdata->in_ax[count] != '\0')
	{
		if ((prs->ptrdata->in_ax[count] == '$' && prs->flag == 0) || (prs->ptrdata->in_ax[count] == '$'
				&& prs->flag == DOUB_QUOT))
                count_dollar++; 
        if (prs->ptrdata->in_ax[count] == DOUB_QUOT)
			ft_if_doub_quote_exp(prs);
		else if (prs->ptrdata->in_ax[count] == ONE_QUOT)
			ft_if_single_quote_exp(prs);
		count++;
	}
    ft_expan_n_times(prs, count_dollar);
}

void		ft_expan_n_times(t_parsing *prs, int times)
{
 	int	count;
    int cnt_cntrl;

    count = 0;
    cnt_cntrl = 0;	
    prs->flag = 0;
	while(prs->ptrdata->in_ax[count] != '\0' && cnt_cntrl < times)
	{
		if ((prs->ptrdata->in_ax[count] == '$' && prs->flag == 0) || (prs->ptrdata->in_ax[count] == '$'
				&& prs->flag == DOUB_QUOT))
        {
            cnt_cntrl++;
            ft_dollar_by_dollar(prs, count);
        }        
		if (prs->ptrdata->in_ax[count] == DOUB_QUOT)
			ft_if_doub_quote_exp(prs);
		else if (prs->ptrdata->in_ax[count] == ONE_QUOT)
			ft_if_single_quote_exp(prs);
		count++;
	}
}

void		ft_dollar_by_dollar(t_parsing *prs, int count)
{
    char    *var_name;
    var_name = ft_take_name_var(prs, count);
    printf("hola: %s\n", var_name);
    free(var_name);
    var_name = NULL;
}


char    *ft_take_name_var(t_parsing *prs, int cnt_str)
{
    char *var_name;
    int start_str;
    int len;
    int count;
    
    count = 0;
    if (prs->ptrdata->in_ax[cnt_str] == '$')
    {
        cnt_str++; 
        start_str = cnt_str;
        while (ft_isalnum(prs->ptrdata->in_ax[cnt_str]) == 1 ||
               prs->ptrdata->in_ax[cnt_str] == '{' ||
               prs->ptrdata->in_ax[cnt_str] == '}' ||
               prs->ptrdata->in_ax[cnt_str] == '_')
            cnt_str++;
        len = cnt_str - start_str;
        var_name = malloc(sizeof(char) * (len + 1));
        if (!var_name)
        {
		    printf("String has not been created (Error).");
		    exit(0);
	    }
        while (count < len)
        {
            var_name[count] = prs->ptrdata->in_ax[start_str + count];
            count++;
        }
        var_name[len] = '\0'; 
    }
    return (var_name);
}

// si está función devuelve 0 SÍ expandimos, de lo contrario no!

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

char *ft_strjoin_insert(const char *s1, const char *add, size_t insrt_indx, int k)
{
    size_t len1;  
    size_t len2; 
    char *result;
    size_t i;
    size_t j; 
    
    i = 0;
    j = 0;
    k = 0;
    len1 = ft_strlen(s1);
    len2 = ft_strlen(add);
    result = malloc(sizeof(char) * (len1 + len2 + 1));
    if (!result)
    {
	    printf("String has not been created (Error).");
	    exit(0);
    }
    while (i < insrt_indx && i < len1)
    {
        result[k] = s1[i];
        k++;
        i++;
    }
    while (j < len2)
    {
        result[k] = add[j];
        k++;
        j++;
    }
    while (i < len1)
    {
        result[k] = s1[i];
        k++;
        i++;
    }
    result[k] = '\0';
    return (result);
}


// MISMA IDEA PERO CON ARRAY DE STRINGS:
/*
void	ft_check_if_expans_var(t_parsing *prs)
{
	int	cnt_arr;
	int	cnt_str;
	int	cnt_dollar;

	cnt_arr = 0;
	cnt_str = 0;
	cnt_dollar = 0;
	while (prs->arr_lexems[cnt_arr] != NULL)
	{
		cnt_str = 0;
		while (prs->arr_lexems[cnt_arr][cnt_str] != '\0')
		{
			if (prs->arr_lexems[cnt_arr][cnt_str] == '$')
			{
				cnt_dollar++;
				prs->flag = 0;
				if (ft_check_dollar(prs, cnt_dollar) == 0)
                {
					printf("¡¡ Aquí sí expande, acordarme de $?  !!%s\n", prs->arr_lexems[cnt_arr]);
                    ft_take_name_var(prs, cnt_arr, cnt_str);
                }
				else
                    printf("Aquí NO expande");
			}
			cnt_str++;
		}
		cnt_arr++;
	}
}

void ft_take_name_var(t_parsing *prs, int cnt_arr, int cnt_str)
{
    char *var_name;
    int start_str;
    int len;
    int count;
    
    count = 0;
    if (prs->arr_lexems[cnt_arr][cnt_str] == '$')
    {
        cnt_str++; 
        start_str = cnt_str;
        while (ft_isalnum(prs->arr_lexems[cnt_arr][cnt_str]) == 1 ||
               prs->arr_lexems[cnt_arr][cnt_str] == '{' ||
               prs->arr_lexems[cnt_arr][cnt_str] == '}' ||
               prs->arr_lexems[cnt_arr][cnt_str] == '_')
            cnt_str++;
        len = cnt_str - start_str;
        var_name = malloc(sizeof(char) * (len + 1));
        if (!var_name)
        {
		    printf("String has not been created (Error).");
		    exit(0);
	    }
        while (count < len)
        {
            var_name[count] = prs->arr_lexems[cnt_arr][start_str + count];
            count++;
        }
        var_name[len] = '\0'; 
        printf("\nVariable name: %s\n", var_name);
        free(var_name);
        var_name = NULL;
    }
}

// si esta función devuelve 0 SÍ expandimos, de lo contrario no!
int	ft_check_dollar(t_parsing *prs, int index)
{
	int	count;
	int	cnt_dol;

	cnt_dol = 0;
	count = 0;
	while (prs->ptrdata->in_ax[count] != '\0')
	{
		if (prs->ptrdata->in_ax[count] == '$')
			cnt_dol++;
		if (prs->ptrdata->in_ax[count] == DOUB_QUOT)
			ft_if_doub_quote_exp(prs);
		else if (prs->ptrdata->in_ax[count] == ONE_QUOT)
			ft_if_single_quote_exp(prs);
        if ((cnt_dol == index && prs->flag == ONE_QUOT))
            return (1);
		if ((cnt_dol == index && prs->flag == 0) || (cnt_dol == index
				&& prs->flag == DOUB_QUOT))
			return (0);
		count++;
	}
	return (1);
}

// si está función devuelve 0 SÍ expandimos, de lo contrario no!

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

char *ft_strjoin_insert(const char *s1, const char *add, size_t insrt_indx, int k)
{
    size_t len1;  
    size_t len2; 
    char *result;
    size_t i;
    size_t j; 
    
    i = 0;
    j = 0;
    k = 0;
    len1 = ft_strlen(s1);
    len2 = ft_strlen(add);
    result = malloc(sizeof(char) * (len1 + len2 + 1));
    if (!result)
    {
	    printf("String has not been created (Error).");
	    exit(0);
    }
    while (i < insrt_indx && i < len1)
    {
        result[k] = s1[i];
        k++;
        i++;
    }
    while (j < len2)
    {
        result[k] = add[j];
        k++;
        j++;
    }
    while (i < len1)
    {
        result[k] = s1[i];
        k++;
        i++;
    }
    result[k] = '\0';
    return (result);
}
*/