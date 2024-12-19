/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_4.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/27 10:31:20 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/13 14:55:48 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

// esta función nos dirá si un carácter concreto (index) de una string está o
// no está en estado de comillas. Si está FUERA de comillas devolverá 1, si
// está dentro de comillas devolverá 0
// en general todo este archivo consigue que se separen los carácteres 
// especiales de otros lexemas.

int	ft_is_spc_chr(char str)
{
	if (str == '>' || str == '<' || str == '|')
		return (1);
	return (0);
}

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
}

int	ft_len(char *s, int k, int i, int add)
{
	if (!s)
		return (0);
	while (s[i] != '\0')
	{
		if (((i > 0 && ((s[i] == '>' && s[i + 1] == '>')
						|| (s[i] == '<' && s[i + 1] == '<')))
				&& ft_tell_if_oq(s, i, k, 0) == 0
				&& s[i - 1] != ' ') || (i > 0 && ft_is_spc_chr(s[i]) == 1
				&& ft_tell_if_oq(s, i, k, 0) == 0 && s[i - 1] != ' ') || (i >= 2
				&& s[i] != ' ' && ((s[i - 2] == '>' && s[i - 1] == '>')
					|| (s[i - 2] == '<' && s[i - 1] == '<')) && s[i] != '\0'
				&& ft_tell_if_oq(s, i - 2, k, 0) == 0) || (i >= 1 && s[i] != ' '
				&& ft_is_spc_chr(s[i - 1]) == 1 && s[i] != '\0'
				&& ft_tell_if_oq(s, i - 1, k, 0) == 0))
			add++;
		if ((s[i] == '>' && s[i + 1] == '>') || (s[i] == '<' && s[i
					+ 1] == '<'))
			i += 2;
		else
			i++;
	}
	return (add);
}

char	*ft_new_input(char *s, int len)
{
	int		i;
	char	*new_str;
	int		cnt_ns;

	i = 0;
	cnt_ns = 0;
	new_str = malloc(sizeof(char) * len + 1);
	if (!new_str)
	{
		printf("String has not been created (Error)");
		exit(0);
	}
	new_str = ft_new_input_aux(s, new_str, i, cnt_ns);
	new_str[len] = '\0';
	return (new_str);
}

char	*ft_new_input_aux(char *s, char *new_str, int i, int cnt_ns)
{
	while (s[i] != '\0')
	{
		if (((i > 0 && ((s[i] == '>' && s[i + 1] == '>')
						|| (s[i] == '<' && s[i + 1] == '<')))
				&& ft_tell_if_oq(s, i, 0, 0) == 0
				&& s[i - 1] != ' ') || (i > 0 && ft_is_spc_chr(s[i]) == 1
				&& ft_tell_if_oq(s, i, 0, 0) == 0 && s[i - 1] != ' ') || (i >= 2
				&& s[i] != ' ' && ((s[i - 2] == '>' && s[i - 1] == '>')
					|| (s[i - 2] == '<' && s[i - 1] == '<')) && s[i] != '\0'
				&& ft_tell_if_oq(s, i - 2, 0, 0) == 0) || (i >= 1 && s[i] != ' '
				&& ft_is_spc_chr(s[i - 1]) == 1 && s[i] != '\0'
				&& ft_tell_if_oq(s, i - 1, 0, 0) == 0))
			new_str[cnt_ns++] = ' ';
		new_str[cnt_ns] = s[i];
		if ((s[i] == '>' && s[i + 1] == '>') || (s[i] == '<' && s[i
					+ 1] == '<'))
		{
			i++;
			cnt_ns++;
			new_str[cnt_ns] = s[i];
		}
		i++;
		cnt_ns++;
	}
	return (new_str);
}
