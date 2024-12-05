#include "../../include/minishell.h"

void	delete_var(char **array, char *var_name)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	len = ft_strlen(var_name);
	while (array[i])
	{
		if (ft_strncmp(array[i], var_name, len) == 0 && array[i][len] == '=')
		{
			free(array[i]);
			j = i;
			while (array[j])
			{
				array[j] = array[j + 1];
				j++;
			}
			return ;
		}
		i++;
	}
}

void	ft_unset(t_cmd *cmd)
{
	int	i;

	i = 1;
	while (cmd->array_cmd[i])
	{
		delete_var(cmd->data->env, cmd->array_cmd[i]);
		delete_var(cmd->data->array_var, cmd->array_cmd[i]);
		i++;
	}
	builtin_end(cmd->data, 0);
}
