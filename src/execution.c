#include "../include/minishell.h"

void execution (t_data *data)
{
    printf("\n---------EJECUCION---------\n\n");
    printf("Resultado de nuestros builtins:\n");
    builtins(data);
    //Esto ejecuta un comando y por tanto acaba el proceso!!!
    //prueba_ejecucion(data);
    // char *path = get_path(data->array_input[0], data->env);
    // printf("Resultado con execve:\n");
    // execve(path, data->array_input, data->env);
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
void one_cmd_case(t_data *data)
{
  
  pid_t pid;
    char *path;

    pid = fork();
    if(pid == 0)
    {
        path = get_path(data->cmd_list->array_cmd[0], data->env);
        execve(path, data->cmd_list->array_cmd, data->env);
    }
    else
        waitpid(pid, &data->exit_status, 0);
}
// void multiple_cmd_case(t_data *data) //PROPUESTO POR COPILOT
// {
//     t_cmd *cmd;
//     int fd[2];
//     int fd_in;
//     int fd_out;
//     pid_t pid;
//     char *path;

//     cmd = data->cmd_list;
//     fd_in = 0;
//     while(cmd)
//     {
//         pipe(fd);
//         pid = fork();
//         if(pid == 0)
//         {
//             if(cmd->next)
//                 fd_out = fd[1];
//             else
//                 fd_out = 1;
//             dup2(fd_in, 0);
//             dup2(fd_out, 1);
//             close(fd[0]);
//             path = get_path(cmd->array_cmd[0], data->env);
//             execve(path, cmd->array_cmd, data->env);
//         }
//         else
//         {
//             waitpid(pid, &data->exit_status, 0);
//             close(fd[1]);
//             fd_in = fd[0];
//             cmd = cmd->next;
//         }
//     }
// }
void print_pipe(int *pipe)
{
    printf("pipe[0]: %d\n", pipe[0]);
    printf("pipe[1]: %d\n", pipe[1]);
}
int is_last_cmd(t_cmd *cmd)
{
    if(cmd->next == NULL)
        return (1);
    return (0);
}
int is_first_cmd(t_cmd *cmd)
{
    if(cmd->index == 0)
        return (1);
    return (0);
}
void child(t_cmd *cmd, int *pipe)
{
    char *path;
    if(is_first_cmd(cmd))
        dup2(0, STDIN_FILENO);
    else
        dup2(pipe[1], STDIN_FILENO); //Hace que el de entrada (STDIN_FILENO) sea la salida del anterior poniendolo en la pipe
    close(pipe[0]);
    if(is_last_cmd(cmd))
        dup2(1, STDOUT_FILENO);
    else
        dup2(pipe[0], STDOUT_FILENO); //Hace que el de salida (STDOUT_FILENO) sea la entrada del siguiente poniendolo en la pipe
    path = get_path(cmd->array_cmd[0], cmd->data->env);
    execve(path, cmd->array_cmd, cmd->data->env);
}


void multiple_cmd_case(t_data *data)
{
    //print_cmd_list(data->cmd_list);
    pid_t pid;
    t_cmd *cmd;
    // int fd_in;

    // fd_in = 0;
    data->pipe = malloc(sizeof(int) * 2);
    data->pipe[0] = 0;
    data->pipe[1] = 1;
    cmd = data->cmd_list;
    pipe(data->pipe);
    // print_cmd_list(data->cmd_list);
    while(cmd)
    {
        pid = fork();
        if(pid == 0)
        {
            child(cmd, data->pipe);
        }
        waitpid(pid, &data->exit_status, 0);
        close(data->pipe[1]);//cierra salida en el padre
        cmd = cmd->next;
    }
}

// void prueba_ejecucion(t_data *data)
// {
//     // data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split("ls -la", ' ')));
//     // data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split("grep test", ' ')));
//     // data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split("wc -l", ' ')));
//     // if(cmd_list_len(data->cmd_list) == 1)
//     //     one_cmd_case(data);
//     // else
//     //     multiple_cmd_case(data);
// }
//pid = 0 es el hijo