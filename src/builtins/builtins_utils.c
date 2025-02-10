/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/18 12:22:15 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/23 02:00:46 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	*ft_var_name(char *name_value)
{
	int	i;

	i = 0;
	while (name_value[i] != '=' && name_value[i] != '\0')
		i++;
	return (ft_substr(name_value, 0, i));
}

char	*ft_var_value(char *name_value)
{
	int	i;

	i = 0;
	while (name_value[i] != '=' && name_value[i] != '\0')
		i++;
	return (ft_substr(name_value, i + 1, ft_strlen(name_value) - i));
}

char	*remove_brackets(char *var_name)
{
	char	*temp;

	if (var_name[0] == '{' && var_name[ft_strlen(var_name) - 1] == '}')
	{
		temp = ft_substr(var_name, 1, ft_strlen(var_name) - 2);
		var_name = ft_strdup(temp);
		free(temp);
	}
	else
		var_name = ft_strdup(var_name);
	return (var_name);
}

char	*ft_getenv(char *var_name, char **env, t_data *data)
{
	int		i;

	var_name = remove_brackets(var_name);
	i = 0;
	if (ft_strcmp(var_name, "?") == 0)
	{
		free(var_name);
		return (ft_itoa(data->exit_status));
	}
	while (env[i])
	{
		if ((ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0)
			&& env[i][ft_strlen(var_name)] == '=')
		{
			free(var_name);
			return (ft_var_value(env[i]));
		}
		i++;
	}
	free(var_name);
	return (ft_strdup(""));
}

char	**insert_var(char **array, char *var_name, char *var_value)
{
	int		i;
	char	**new_array;
	char	*temp;

	i = 0;
	while (array[i])
	{
		if (ft_strncmp(array[i], var_name, ft_strlen(var_name)) == 0)
		{
			free(array[i]);
			temp = ft_strjoin(var_name, "=");
			array[i] = ft_strjoin(temp, var_value);
			free(temp);
			return (array);
		}
		i++;
	}
	new_array = realloc_elonged_array(array);
	temp = ft_strjoin(var_name, "=");
	new_array[i] = ft_strjoin(temp, var_value);
	new_array[i + 1] = NULL;
	free_array(array);
	free(temp);
	return (new_array);
}
