#include "../include/minishell.h"

void print_env(char **array)
{
    int i = 0;
    while (array[i])
    {
        printf("%s\n", array[i]);
        i++;
    }
}

void ft_env(t_cmd *cmd)
{
    print_env(cmd->data->env);
}