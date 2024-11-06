#include "../include/minishell.h"

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
void exec_builtin(t_cmd *cmd) //Esto hay que reescribirlo para que funcione con la estructura de t_cmd
{
    if(ft_strcmp(cmd->array_cmd[0], "echo") == 0)
        ft_echo(cmd);
    else if(ft_strcmp(cmd->array_cmd[0], "pwd") == 0)
        ft_pwd();
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

void child(t_cmd *cmd, int *fd_in, int *fd_out, t_data *data)
{
    char *path;
    // Configura la salida del proceso hijo
    if (is_last_cmd(cmd) == 0) // Si hay un comando siguiente
        *fd_out = data->pipe[1]; // Redirige la salida a la escritura de la tubería
    else
        *fd_out = 1; // Si es el último comando, redirige a la salida estándar
    dup2(*fd_in, 0); // Redirige la entrada estándar a la salida del comando anterior
    dup2(*fd_out, 1); // Redirige la salida estándar al fd_out seleccionado
    close(data->pipe[0]); // Cierra el extremo de lectura de la tubería en el hijo
    path = get_path(cmd->array_cmd[0], data->env); // Obtiene la ruta del comando
    execve(path, cmd->array_cmd, data->env); // Ejecuta el comando con execve
    //poner algo por si execve falla
}

void execution(t_data *data)
{
    t_cmd *cmd;
    int fd_in;
    int fd_out;
    pid_t pid;

    fd_in = 0; // Descriptor de archivo para la entrada del primer comando
    cmd = data->cmd_list;
    while (cmd)
    {
        pipe(data->pipe); // Crea la tubería para el comando actual y el siguiente
        pid = fork(); // Crea un proceso hijo para ejecutar el comando

        if (pid == 0) //Hijo
            child(cmd, &fd_in, &fd_out, data);
        else //Padre
        {
            waitpid(pid, &data->exit_status, 0); // Espera a que el hijo termine
            close(data->pipe[1]); // Cierra el extremo de escritura de la tubería en el padre
            // Actualiza fd_in para el próximo comando, usando el extremo de lectura actual
            fd_in = data->pipe[0]; 
            cmd = cmd->next;
        }
    }
}

void prueba_ejecucion(t_data *data)
{
    // data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split("ls -la", ' ')));
    // data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split("grep test", ' ')));
    // data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split("wc -l", ' ')));
    // multiple_cmd_case(data);
    data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split(data->input, ' '), data));
    printf("is_a_builtin: %d\n", is_a_builtin(data->cmd_list));
    exec_builtin(data->cmd_list);

}
