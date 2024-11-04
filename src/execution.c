#include "../include/minishell.h"

void execution (t_data *data)
{
    printf("\n---------EJECUCION---------\n\n");
    //printf("Resultado de nuestros builtins:\n");
    // if (builtins(data) == 1)
    //     return;
    //Esto ejecuta un comando y por tanto acaba el proceso!!!
    char *path = get_path(data->array_input[0], data->env);
    printf("Resultado con execve:\n");
    execve(path, data->array_input, data->env);
    //Solo funciona si solo le pasamos un comando. Si le pasamos un comando con argumentos no funciona.
}
//!!!!!!!!!!!!!!!!!!Hay que hacer que devulevan los mismos errores!!!!!!!!!!!!!!!!!!!
void builtins(t_data *data)
{
    if(ft_strcmp(data->array_input[0], "echo") == 0)
        ft_echo(data);
    else if(ft_strcmp(data->array_input[0], "pwd") == 0)
        ft_pwd();
    else if (ft_strcmp(data->array_input[0], "cd") == 0)
        ft_cd(data);
    else if (ft_strcmp(data->array_input[0], "env") == 0)
        ft_env(data);
    else if (ft_strcmp(data->array_input[0], "export") == 0)
        ft_export(data);
    else if (ft_strcmp(data->array_input[0], "unset") == 0)
        ft_unset(data);
}