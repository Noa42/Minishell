/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 10:57:05 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/22 10:22:59 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	print_export(char **array_var)
{
	int		i;
	char	*var_name;
	char	*var_value;

	i = 0;
	while (array_var[i])
	{
		var_name = ft_var_name(array_var[i]);
		var_value = ft_var_value(array_var[i]);
		if (ft_strlen(var_name) == 0)
			printf("declare -x %s\n", var_name);
		else
			printf("declare -x %s=\"%s\"\n", var_name, var_value);
		i++;
		free(var_name);
		free(var_value);
	}
}

int	valid_var_name(char *var_name)
{
	int	i;

	i = 0;
	if (!ft_isalpha(var_name[i]) && var_name[i] != '_')
		return (0);
	i++;
	while (var_name[i])
	{
		if (!ft_isalnum(var_name[i]) && var_name[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

void	not_valid_identifier(char *var_name, int *exit_status)
{
	ft_putstr_fd("MiniShell: export: `", 2);
	ft_putstr_fd(var_name, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
	free(var_name);
	*exit_status = 1;
}

void	valid_identifier(t_cmd *cmd, char *var_name, int i)
{
	char	*var_value;

	var_value = ft_var_value(cmd->array_cmd[i]);
	cmd->data->env = insert_var(cmd->data->env, var_name, var_value);
	cmd->data->array_var = \
	insert_var(cmd->data->array_var, var_name, var_value);
	free(var_name);
	free(var_value);
}

void	ft_export(t_cmd *cmd)
{
	char	*var_name;
	int		i;
	int		exit_status;

	exit_status = 0;
	if (cmd->array_cmd[1] == NULL)
	{
		print_export(cmd->data->array_var);
		builtin_end(cmd->data, 0);
		return ;
	}
	i = 1;
	while (cmd->array_cmd[i])
	{
		var_name = ft_var_name(cmd->array_cmd[i]);
		if (valid_var_name((var_name)) == 0)
		{
			not_valid_identifier(var_name, &exit_status);
			break ;
		}
		valid_identifier(cmd, var_name, i);
		i++;
	}
	builtin_end(cmd->data, exit_status);
}
