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

void	ft_export(t_cmd *cmd)
{
	char	*var_name;
	char	*var_value;
	int		i;

	if (cmd->array_cmd[1] == NULL)
	{
		print_export(cmd->data->array_var);
		return ;
	}
	i = 1;
	while (cmd->array_cmd[i])
	{
		var_name = ft_var_name(cmd->array_cmd[i]);
		var_value = ft_var_value(cmd->array_cmd[i]);
		//if comprobaciones porque hay variables que se incluyen en export y no en env por ejemplo si está vacía $export a="", esto aparece en export pero no en env
		cmd->data->env = insert_var(cmd->data->env, var_name, var_value);
		cmd->data->array_var = \
		insert_var(cmd->data->array_var, var_name, var_value);
		i++;
		free(var_name);
		free(var_value);
	}
	builtin_end(cmd->data, 0);
}
