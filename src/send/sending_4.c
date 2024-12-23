/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sending_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 00:50:42 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/23 00:52:01 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	ft_tell_me_if_pipes(t_parsing *prs)
{
	int	i;
	int	count;

	count = 0;
	i = 0;
	while (prs->arr_lexems && prs->arr_lexems[i] != NULL)
	{
		if (prs->arr_lexems[i] && ft_strlen(prs->arr_lexems[i]) == 1
			&& prs->arr_lexems[i][0] == '|')
			count++;
		i++;
	}
	return (count);
}
