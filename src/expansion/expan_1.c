/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/20 13:34:14 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/23 02:05:40 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_start_expas(t_parsing *prs)
{
	int	count;
	int	count_dollar;

	count = 0;
	count_dollar = 0;
	prs->flag = 0;
	while (prs->ptrdata->in_ax[count] != '\0')
	{
		if ((prs->ptrdata->in_ax[count] == '$' && prs->flag == 0)
			|| (prs->ptrdata->in_ax[count] == '$' && prs->flag == DOUB_QUOT))
			count_dollar++;
		if (prs->ptrdata->in_ax[count] == DOUB_QUOT)
			ft_if_doub_quote_exp(prs);
		else if (prs->ptrdata->in_ax[count] == ONE_QUOT)
			ft_if_single_quote_exp(prs);
		count++;
	}
	ft_expan_n_times(prs, count_dollar);
}

void	ft_expan_n_times(t_parsing *prs, int times)
{
	size_t	count;
	int		cnt_cntrl;

	count = 0;
	cnt_cntrl = 0;
	prs->flag = 0;
	while (prs->ptrdata->in_ax[count] != '\0' && cnt_cntrl < times)
	{
		if ((prs->ptrdata->in_ax[count] == '$' && prs->flag == 0)
			|| (prs->ptrdata->in_ax[count] == '$' && prs->flag == DOUB_QUOT))
		{
			cnt_cntrl++;
			ft_dollar_by_dollar(prs, count);
		}
		if (count >= ft_strlen(prs->ptrdata->in_ax))
			break ;
		if (prs->ptrdata->in_ax[count] == DOUB_QUOT)
			ft_if_doub_quote_exp(prs);
		else if (prs->ptrdata->in_ax[count] == ONE_QUOT)
			ft_if_single_quote_exp(prs);
		count++;
	}
}

char	*ft_look_for_var_value(char **env, const char *var_name)
{
	size_t	len;
	int		count;

	len = ft_strlen(var_name);
	count = 0;
	while (env[count])
	{
		if (ft_strncmp(env[count], var_name, len) == 0
			&& env[count][len] == '=')
			return (env[count] + len + 1);
		count++;
	}
	return (NULL);
}

char	*ft_return_aux(t_parsing *prs, int count)
{
	char	*var_name;
	char	*aux;
	char	*var_value;
	char	*str2;

	var_name = ft_take_name_var(prs, count);
	str2 = remove_brackets(var_name);
	if (!var_name || var_name[0] == '\0')
		aux = ft_strjoin_insert(prs->ptrdata->in_ax, "$", count, 1);
	else
	{
		var_value = ft_look_for_var_value(prs->ptrdata->env, str2);
		if (!var_value)
			aux = ft_strjoin_insert(prs->ptrdata->in_ax, "", count,
					ft_strlen(var_name) + 1);
		else
			aux = ft_strjoin_insert(prs->ptrdata->in_ax, var_value, count,
					ft_strlen(var_name) + 1);
	}
	free(var_name);
	free(str2);
	return (aux);
}

void	ft_dollar_by_dollar(t_parsing *prs, int count)
{
	char	*aux;
	char	*aux_nbr;

	aux_nbr = NULL;
	if (prs->ptrdata->in_ax[count + 1] == '?')
	{
		aux_nbr = ft_itoa(prs->ptrdata->exit_status);
		aux = ft_strjoin_insert(prs->ptrdata->in_ax, aux_nbr, count, 2);
	}
	else if (prs->ptrdata->in_ax[count + 1] == '{' && prs->ptrdata->in_ax[count
			+ 2] == '?' && prs->ptrdata->in_ax[count + 3] == '}')
	{
		aux_nbr = ft_itoa(prs->ptrdata->exit_status);
		aux = ft_strjoin_insert(prs->ptrdata->in_ax, aux_nbr, count, 4);
	}
	else if (ft_condition(prs, count))
		aux = ft_strjoin_insert(prs->ptrdata->in_ax, "", count, 3);
	else
		aux = ft_return_aux(prs, count);
	free(prs->ptrdata->in_ax);
	prs->ptrdata->in_ax = ft_strdup(aux);
	free(aux);
	free(aux_nbr);
}
