/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expan_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 00:24:13 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/23 02:05:37 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_take_name_var(t_parsing *prs, int cnt_str)
{
	char	*var_name;
	int		start_str;
	int		len;
	int		count;

	count = 0;
	if (prs->ptrdata->in_ax[cnt_str] == '$')
	{
		cnt_str++;
		start_str = cnt_str;
		while (ft_isalnum(prs->ptrdata->in_ax[cnt_str]) == 1
			|| prs->ptrdata->in_ax[cnt_str] == '{'
			|| prs->ptrdata->in_ax[cnt_str] == '}'
			|| prs->ptrdata->in_ax[cnt_str] == '_')
			cnt_str++;
		len = cnt_str - start_str;
		var_name = malloc(sizeof(char) * (len + 1));
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

char	*ft_strjoin_insert(const char *s1, const char *add, size_t insrt_indx,
		size_t del_len)
{
	char	*result;
	size_t	len1;
	size_t	len2;
	size_t	count_s2;
	size_t	count_ns;

	count_s2 = 0;
	count_ns = 0;
	len1 = ft_strlen(s1);
	len2 = ft_strlen(add);
	result = ft_calloc(len1 + len2 + 1, sizeof(char));
	while (count_ns < insrt_indx && count_ns < len1)
	{
		result[count_ns] = s1[count_ns];
		count_ns++;
	}
	while (count_s2 < len2)
		result[count_ns++] = add[count_s2++];
	count_s2 = insrt_indx + del_len;
	while (count_s2 < len1)
		result[count_ns++] = s1[count_s2++];
	return (result);
}

int	ft_condition(t_parsing *prs, int count)
{
	return ((prs->ptrdata->in_ax[count + 1] == '\'' && prs->ptrdata->in_ax[count
				+ 2] == '\'') || (prs->ptrdata->in_ax[count + 1] == '\"'
			&& prs->ptrdata->in_ax[count + 2] == '\"'));
}
