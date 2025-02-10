/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 12:21:59 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/23 00:24:41 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	empty_env(t_data *data)
{
	char	*pwd;

	pwd = malloc(sizeof(char) * 1024);
	getcwd(pwd, 1024);
	data->env = malloc(sizeof(char *) * 5);
	data->env[0] = ft_strdup("OLDPWD");
	data->env[1] = ft_strjoin("PWD=", pwd);
	data->env[2] = ft_strdup("SHLVL=1");
	data->env[3] = ft_strdup("_=EMPTY");
	data->env[4] = NULL;
	data->array_var = copy_alloc_array(data->env);
	free(pwd);
}

void	update_shlvl(t_data *data)
{
	int		shlvl_int;
	char	*shlvl_str;

	shlvl_str = ft_getenv("SHLVL", data->env, data);
	shlvl_int = ft_atoi(shlvl_str);
	shlvl_int ++;
	free(shlvl_str);
	shlvl_str = ft_itoa(shlvl_int);
	insert_var(data->env, "SHLVL", shlvl_str);
	insert_var(data->array_var, "SHLVL", shlvl_str);
	free(shlvl_str);
}
