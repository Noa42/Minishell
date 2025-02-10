/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 12:22:41 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/23 02:00:20 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_variable(char *input)
{
	int	i;	
	int	cnt_dollar;

	cnt_dollar = 0;
	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '$' && input[i + 1] != '\0' && input[i + 1] != ' ')
			cnt_dollar++;
		i++;
	}
	return (cnt_dollar);
}

char	*get_exp_var(char *s, int i)
{
	unsigned int	start;
	char			*var;

	start = i;
	while (s[i] != ' ' && s[i] != '\0')
		i++;
	var = ft_substr(s, start, i - start);
	return (var);
}

void	set_n_flag_and_i(char **array_cmd, int *n_flag, int *i)
{
	int		j;
	char	*s;

	j = 1;
	while (array_cmd [*i] != NULL)
	{
		s = array_cmd[*i];
		j = 1;
		while (s[j] != '\0' && s[j] == 'n')
			j++;
		if (s[j] == '\0')
		{
			*n_flag = 1;
			*i += 1;
		}
		else
			break ;
	}
}

void	ft_echo(t_cmd *cmd)
{
	int	n_flag;
	int	i;

	i = 1;
	n_flag = 0;
	if (array_len(cmd->array_cmd) >= 2 && ft_strlen(cmd->array_cmd[1]) >= 2)
		set_n_flag_and_i(cmd->array_cmd, &n_flag, &i);
	while (cmd->array_cmd[i] != NULL)
	{
		ft_printf("%s", cmd->array_cmd[i]);
		if (cmd->array_cmd[i + 1] != NULL)
			ft_printf(" ");
		i++;
	}
	if (n_flag == 0)
		ft_printf("\n");
	builtin_end(cmd->data, 0);
}
