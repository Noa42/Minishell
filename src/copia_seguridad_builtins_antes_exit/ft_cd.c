#include "../include/minishell.h"

void ft_cd_home(t_data *data, char* oldpwd, char* pwd)
{
    char *home;

    home = ft_getenv("HOME", data->env);
    if (chdir(home) != 0)
    {
        free(oldpwd);
        free(pwd);
        printf("Error cambiando de dirección\n");
        return;
    }
    else
    {
        data->env = insert_var(data->env, "OLDPWD", oldpwd);
        data->array_var = insert_var(data->array_var, "OLDPWD", oldpwd);
        getcwd(pwd, 1024);
        data->env = insert_var(data->env, "PWD", pwd);
        data->array_var = insert_var(data->array_var, "PWD", pwd);
        free(pwd);
        free(oldpwd);
    }
}
void ft_cd(t_cmd *cmd)
{
    char *oldpwd;
    char *pwd;
    
    oldpwd = malloc(sizeof(char)* 1024);
    pwd = malloc(sizeof(char)* 1024);
    getcwd(oldpwd, 1024);
    if (cmd->array_cmd[1] == NULL || ft_strcmp(cmd->array_cmd[1], "~") == 0)
    {
        ft_cd_home(cmd->data, oldpwd, pwd);
        return;
    }
    if (chdir(cmd->array_cmd[1]) != 0)
    {
        free(oldpwd);
        free (pwd);
        printf("Error: %s\n", strerror(errno));//Esto hay que cambiarlo por un mensaje de error
        cmd->data->exit_status = errno;
        return;
    }
    else
    {
        cmd->data->env = insert_var(cmd->data->env, "OLDPWD", oldpwd);
        cmd->data->array_var = insert_var(cmd->data->array_var, "OLDPWD", oldpwd);
        getcwd(pwd, 1024);
        cmd->data->env = insert_var(cmd->data->env, "PWD", pwd);
        cmd->data->array_var = insert_var(cmd->data->array_var, "PWD", pwd);
        free(pwd);
        free(oldpwd);
        cmd->data->exit_status = 0;
    }
}
