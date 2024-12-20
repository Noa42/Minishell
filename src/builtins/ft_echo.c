/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 12:22:41 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/21 14:53:09 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	has_variable(char *input)//Mira si hay un $ en la cadena
{
	int	i;	
	int cnt_dollar;

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

char	*get_exp_var(char *s, int i)//De la palabra que empieza por $ devuelve el nombre de la variable (es decir lo que hay después de $ lo que llamamos var_name)
{
	unsigned int	start;
	char			*var;

	start = i;
	while (s[i] != ' ' && s[i] != '\0')
		i++;
	var = ft_substr(s, start, i - start);
	return (var);
}

void	print_expanded(char *input, char **env)//Imprime una palabra con una variable expandida
{
	int		i;
	char	*var_name;
	char	*var_value;	

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] == '$')
		{
			i++;
			var_name = get_exp_var(input, i);//aloca una subcadena desde despues de $ hasta el siquiente espacio o \0 por lo que devuelve el nombre de la variable a imprimir
			var_value = ft_getenv(var_name, env);//obtiene el valor de la variable
			ft_printf("%s", var_value);//imprime el valor de la variable
			free(var_name);
			free(var_value);
			return ;
		}
		else
			ft_printf("%c", input[i]);
		i++;
	}
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

void	ft_echo(t_cmd *cmd) //he usado ft_printf en vez de printf porque por problemas de como guarda el buffer printf no me hacia bien el -n
{
	int	n_flag;
	int	i;

	i = 1;
	n_flag = 0;
	if (array_len(cmd->array_cmd) >= 2 && ft_strlen(cmd->array_cmd[1]) >= 2)
		set_n_flag_and_i(cmd->array_cmd, &n_flag, &i);
	while (cmd->array_cmd[i] != NULL)
	{
		if (ft_strcmp(cmd->array_cmd[i], "$?") == 0)
			ft_printf("Exit status: %i", cmd->data->exit_status);
		else if (has_variable(cmd->array_cmd[i]) == 0) //si no tiene $
			ft_printf("%s", cmd->array_cmd[i]);//imprimimos la palabra tal cual
		else
			print_expanded(cmd->array_cmd[i], cmd->data->env);//imprme la palabra sustituyendo lo que hay después de $ por el valor de la variable 
		if (cmd->array_cmd[i + 1] != NULL) //pone los espacios entre las palabras a menos que sea el final
			ft_printf(" ");
		i++;
	}
	if (n_flag == 0) //Si la flag no se ha cambiado imprime salto de linea//
		ft_printf("\n");
	builtin_end(cmd->data, 0);
}
