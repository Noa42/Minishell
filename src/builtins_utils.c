#include "../include/minishell.h"

void builtin_end(t_data *data, int error)
{
    if (error == 0)
        data->exit_status = 0;
    else
    {
        ft_printf("Error: ", 2);
        ft_printf(strerror(error), 2);
        ft_printf("\n", 2);
        data->exit_status = error;
    }
    //E2BIG = Argument list too long
}

int is_a_builtin(t_cmd *cmd)
{
    if((ft_strcmp(cmd->array_cmd[0], "echo") == 0) ||
    (ft_strcmp(cmd->array_cmd[0], "pwd") == 0) ||
    (ft_strcmp(cmd->array_cmd[0], "cd") == 0) ||
    (ft_strcmp(cmd->array_cmd[0], "env") == 0) ||
    (ft_strcmp(cmd->array_cmd[0], "export") == 0) ||
    (ft_strcmp(cmd->array_cmd[0], "unset") == 0) ||
    (ft_strcmp(cmd->array_cmd[0], "exit") == 0))
        return (1);
    else
        return (0);
}
void exec_builtin(t_cmd *cmd)
{
    if(ft_strcmp(cmd->array_cmd[0], "echo") == 0)
        ft_echo(cmd);
    else if(ft_strcmp(cmd->array_cmd[0], "pwd") == 0)
        ft_pwd(cmd);
    else if (ft_strcmp(cmd->array_cmd[0], "cd") == 0)
        ft_cd(cmd);
    else if (ft_strcmp(cmd->array_cmd[0], "env") == 0)
        ft_env(cmd);
    else if (ft_strcmp(cmd->array_cmd[0], "export") == 0)
        ft_export(cmd);
    else if (ft_strcmp(cmd->array_cmd[0], "unset") == 0)
        ft_unset(cmd);
    else if (ft_strcmp(cmd->array_cmd[0], "exit") == 0)
        ft_exit(cmd);
}

char *ft_var_name(char *name_value)
{
    int i;
    
    i = 0;
    while (name_value[i] != '=' && name_value[i] != '\0')
        i++;
    return (ft_substr(name_value, 0, i));
}
char *ft_var_value(char *name_value)//Obtiene el var_value de una var_name en una cadena del array env concreto
{
    int i;

    i = 0;
    while (name_value[i] != '=' && name_value[i] != '\0')
        i++;
    return (ft_substr(name_value, i + 1, ft_strlen(name_value) - i));
}

char *ft_getenv(char *var_name, char **env)//obtiene el valor de una variable de entre todas las variables de entorno a partir de su nombre
{
    int i;
    i = 0;
    while (env[i])
    {
        if ((ft_strncmp(env[i], var_name, ft_strlen(var_name)) == 0) && env[i][ft_strlen(var_name)] == '=') //Si el nombre de la variable coincide con el principio de la cadena y el siguiente caracter es un = entonces es la variable que buscamos
            return (ft_var_value(env[i]));
        i++;
    }
    return ("\0");
}

char **insert_var(char **array, char *var_name, char *var_value)
{
    int i;
    char **new_array;
    char *temp;

    i = 0;
    while(array[i])
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



