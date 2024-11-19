#include "../include/minishell.h"

void exit_process(t_data *data, int exit_status)
{
    free_data(data);
    exit(exit_status);
}

void multiple_cmd_case(t_data *data)
{
    t_cmd *cmd;    
    int fd_in;
    int fd_out;
    pid_t pid;
    int status;

    fd_out = STDOUT_FILENO;// Descriptor de archivo para la salida del último comando
    fd_in = STDIN_FILENO; // Descriptor de archivo para la entrada del primer comando
    cmd = data->cmd_list;

    update_fds_redirs(data->cmd_list);
    while (cmd)
    {
        pipe(data->pipe); // Crea la tubería para el comando actual y el siguiente
        pid = fork(); // Crea un proceso hijo para ejecutar el comando
        if (pid == 0) //Hijo
            child(cmd, &fd_in, &fd_out, data);
        else //Padre
        {
            waitpid(pid, &status, 0); //El exit status es una info que se tiene que interpretar con la macro WIFEXITED
            data->exit_status = WEXITSTATUS(status); //WEXITSTATUS es una macro que devuelve el exit status del hijo
            close(data->pipe[1]); // Cierra el extremo de escritura de la tubería en el padre
            // Actualiza fd_in para el próximo comando, usando el extremo de lectura actual
            fd_in = data->pipe[0];
            cmd = cmd->next;
        }
    }
}

void child(t_cmd *cmd, int *fd_in, int *fd_out, t_data *data)
{
    char *path;
    // Configura la salida del proceso hijo
    if (is_last_cmd(cmd) == 0) // Si hay un comando siguiente
        *fd_out = data->pipe[1]; // Redirige la salida a la escritura de la tubería
    else
        *fd_out = STDOUT_FILENO; // Si es el último comando, redirige a la salida estándar
    dup2(*fd_in, STDIN_FILENO); // Redirige la entrada estándar a la salida del comando anterior
    dup2(*fd_out, STDOUT_FILENO); // Redirige la salida estándar al fd_out seleccionado
    close(data->pipe[0]); // Cierra el extremo de lectura de la tubería en el hijo

    dup_fds_redirs(cmd);

    if(is_a_builtin(cmd) == 1)
    {
        if (cmd->fd_in == -1 || cmd->fd_out == -1)
            exit_process(data, data->exit_status);
        exec_builtin(cmd);
        exit_process(data, data->exit_status);
    }
    else
    {
        if (cmd->fd_in == -1 || cmd->fd_out == -1)
            exit_process(data, data->exit_status);
        path = get_path(cmd->array_cmd[0], data->env); // Obtiene la ruta del comando
        execve(path, cmd->array_cmd, data->env); // Ejecuta el comando con execve
        ft_printf("Comand not found\n", 2);
        exit_process(data, 127);
    }
}

void basic_parsing(t_data *data)
{
    char **array_pipes;
    int i = 0;
    array_pipes = ft_split(data->input, '|');
    while(array_pipes[i])
    {
        data->cmd_list = add_cmd(data->cmd_list, new_cmd(ft_split(array_pipes[i], ' '), data));
        i++;
    }
    free_array(array_pipes);
}
void prueba_ejecucion(t_data *data)
{
    //basic_parsing(data);
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "out_file2.txt"));
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "out_file2.txt"));
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in_file1.txt"));
    // add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in_file2.txt"));
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(HERE_DOC, "delim1"));
    // add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(OUTPUT, "out_file1.txt"));
    if (cmd_list_len(data->cmd_list) >= 2)
    {
        add_redir(get_cmd_by_index(data->cmd_list, 1), new_redir(HERE_DOC, "delim2"));
        add_redir(get_cmd_by_index(data->cmd_list, 1), new_redir(OUTPUT, "out_file2.txt"));
    }
    if (cmd_list_len(data->cmd_list) >= 3)
    {
        add_redir(get_cmd_by_index(data->cmd_list, 2), new_redir(HERE_DOC, "delim3"));
        add_redir(get_cmd_by_index(data->cmd_list, 2), new_redir(OUTPUT, "out_file3.txt"));
    }
    // if (cmd_list_len(data->cmd_list) >= 4)
    // {
    //     add_redir(get_cmd_by_index(data->cmd_list, 3), new_redir(HERE_DOC, "delim4"));
    //     add_redir(get_cmd_by_index(data->cmd_list, 3), new_redir(OUTPUT, "out_file4.txt"));
    // }
    //print_cmd_list(data->cmd_list);
    if (cmd_list_len(data->cmd_list) == 1)
        one_cmd_case(data);
    else
        multiple_cmd_case(data);
}

void one_builtin_case(t_cmd *cmd)
{
    int stdin_copy;
    int stdout_copy;

    stdin_copy = dup(STDIN_FILENO);
    stdout_copy = dup(STDOUT_FILENO);
    if (cmd->fd_in == -1 || cmd->fd_out == -1)
    {
        close(stdin_copy);
        close(stdout_copy);
        return;
    }
    dup_fds_redirs(cmd);
    exec_builtin(cmd);
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
    close(stdin_copy);
    close(stdout_copy);
}

void one_cmd_case(t_data *data)
{
    t_cmd *cmd;
    pid_t pid;
    char *path;
    int status;

    cmd = data->cmd_list;
    update_fds_redirs(data->cmd_list);
    if(is_a_builtin(cmd) == 1)
        one_builtin_case(cmd);
    else
    {
        pid = fork();
        if(pid == 0)
        {
            dup_fds_redirs(cmd);
            if (cmd->fd_in == -1 || cmd->fd_out == -1)
                exit(cmd->data->exit_status);
            path = get_path(cmd->array_cmd[0], data->env);
            execve(path, cmd->array_cmd, data->env);
            ft_printf("Comand not found\n", 2);//esto solo ocurre si el execve falla
            exit(127);
        }
        else
        {
            waitpid(pid, &status, 0); //El exit status es una info que se tiene que interpretar con la macro WIFEXITED
            data->exit_status = WEXITSTATUS(status); //WEXITSTATUS es una macro que devuelve el exit status del hijo
        }
    }
}
