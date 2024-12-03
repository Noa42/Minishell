#include "../include/minishell.h"

void ft_cd_home(t_data *data, char* oldpwd, char* pwd)
{
    char *home;

    home = ft_getenv("HOME", data->env);
    if (chdir(home) != 0)
    {
        free(oldpwd);
        free(pwd);
        builtin_end(data, errno);
        return;
    }
    else
    {
        data->env = insert_var(data->env, "OLDPWD", oldpwd);
        data->array_var = insert_var(data->array_var, "OLDPWD", oldpwd);
        getcwd(pwd, 1024);
        data->env = insert_var(data->env, "PWD", pwd);
        data->array_var = insert_var(data->array_var, "PWD", pwd);
        free(home);
        free(pwd);
        free(oldpwd);
        builtin_end(data, errno);
    }
}
void ft_cd(t_cmd *cmd)
{
    char *oldpwd;
    char *pwd;
    
    if (array_len(cmd->array_cmd) > 2)
    {
        builtin_end(cmd->data, E2BIG); //E2BIG = Argument list too long
        return;
    }
    oldpwd = malloc(sizeof(char)* 1024);
    pwd = malloc(sizeof(char)* 1024);
    if (oldpwd == NULL || pwd == NULL)
    {
        ft_putstr_fd("Cannot allocate memory\n", 2);
        exit_process(cmd->data, 1);
    }
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
        builtin_end(cmd->data, errno);
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
        builtin_end(cmd->data, errno);
    }
}
