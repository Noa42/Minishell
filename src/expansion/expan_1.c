/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:34:14 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/22 21:55:39 by alvapari         ###   ########.fr       */
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
 	size_t	count;
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
        if (count >= ft_strlen(prs->ptrdata->in_ax))
            break;        
		if (prs->ptrdata->in_ax[count] == DOUB_QUOT)
			ft_if_doub_quote_exp(prs);
		else if (prs->ptrdata->in_ax[count] == ONE_QUOT)
			ft_if_single_quote_exp(prs);
		count++;
	}
}

char        *ft_look_for_var_value(char **env, const char *var_name)
{
    size_t len;
    int count;
    
    len = ft_strlen(var_name);
    count = 0;
    while (env[count])
    {
        if (ft_strncmp(env[count], var_name, len) == 0 && env[count][len] == '=')
            return (env[count] + len + 1);
        count++;
    }
    return (NULL);
}


void ft_dollar_by_dollar(t_parsing *prs, int count)
{
    char *var_name;
    char *aux;
    char *var_value;
    char    *aux_nbr;
    
    var_name = NULL;
    aux_nbr = NULL;
    if (prs->ptrdata->in_ax[count + 1] == '?')
    {
        aux_nbr = ft_itoa(prs->ptrdata->exit_status);
        aux = ft_strjoin_insert(prs->ptrdata->in_ax, aux_nbr, count, 2);
    }
    else if ((prs->ptrdata->in_ax[count + 1] == '\'' && prs->ptrdata->in_ax[count + 2] == '\'') 
       || (prs->ptrdata->in_ax[count + 1] == '\"' && prs->ptrdata->in_ax[count + 2] == '\"'))
        aux = ft_strjoin_insert(prs->ptrdata->in_ax, "", count, 3);
    else
    {
        var_name = ft_take_name_var(prs, count);
        if (!var_name || var_name[0] == '\0')
            aux = ft_strjoin_insert(prs->ptrdata->in_ax, "$", count, 1);
        else
        {
            var_value = ft_look_for_var_value(prs->ptrdata->env, var_name);
            if (!var_value)
                aux = ft_strjoin_insert(prs->ptrdata->in_ax, "", count, ft_strlen(var_name) + 1);
            else
                aux = ft_strjoin_insert(prs->ptrdata->in_ax, var_value, count, ft_strlen(var_name) + 1);
        }
    }
    free(prs->ptrdata->in_ax);
    prs->ptrdata->in_ax = NULL;
    prs->ptrdata->in_ax = ft_strdup(aux);
    free(var_name);
    free(aux);
    free(aux_nbr);
    aux_nbr = NULL;
    var_name = NULL;
    aux = NULL;
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

char *ft_strjoin_insert(const char *s1, const char *add, size_t insrt_indx, size_t del_len)
{
    char    *result;
    size_t len1; 
    size_t len2; 
    size_t new_len;
    size_t count_s1;
    size_t count_s2;
    size_t count_ns;
    
    count_s1 = 0;
    count_s2 = 0;
    count_ns = 0;
    len1 = ft_strlen(s1);
    len2 = ft_strlen(add);
    new_len = len1 + len2 - del_len;
    result = malloc(sizeof(char) * (new_len + 1));
    if (!result)
    {
        printf("String has not been created (Error).");
        exit(0);
    }
    while (count_s1 < insrt_indx && count_s1 < len1)
    {
        result[count_ns] = s1[count_s1];
        count_ns++;
        count_s1++;
    }
    while (count_s2 < len2)
    {
        result[count_ns] = add[count_s2];
        count_ns++;
        count_s2++;
    }
    count_s1 = insrt_indx + del_len;
    while (count_s1 < len1)
    {
        result[count_ns] = s1[count_s1];
        count_ns++;
        count_s1++;
    }
    result[count_ns] = '\0';
    return (result);
}
