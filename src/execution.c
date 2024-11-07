#include "../include/minishell.h"

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
    if(is_a_builtin(cmd) == 1)
    {
        exec_builtin(cmd);
        exit_process(data, data->exit_status);
    }
    
    else
    {
        path = get_path(cmd->array_cmd[0], data->env); // Obtiene la ruta del comando
        execve(path, cmd->array_cmd, data->env); // Ejecuta el comando con execve
        printf("Comand not found\n");
        exit(127);
    }
}

void multiple_cmd_case(t_data *data)
{
    t_cmd *cmd;
    int fd_in;
    int fd_out;
    pid_t pid;
    int status;

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
            waitpid(pid, &status, 0); //El exit status es una info que se tiene que interpretar con la macro WIFEXITED
            data->exit_status = WEXITSTATUS(status); //WEXITSTATUS es una macro que devuelve el exit status del hijo
            close(data->pipe[1]); // Cierra el extremo de escritura de la tubería en el padre
            // Actualiza fd_in para el próximo comando, usando el extremo de lectura actual
            fd_in = data->pipe[0]; 
            cmd = cmd->next;
        }
    }
}

void apply_INPUT_redir(t_cmd *cmd, t_redir *redir)
{
    cmd->fd_in = open(redir->in_name, O_RDONLY);
    if (cmd->fd_in == -1)
    {
        printf("Error opening file\n");
        exit(1);
    }
}
void apply_OUTPUT_redir(t_cmd *cmd, t_redir *redir)
{
    cmd->fd_out = open(redir->out_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (cmd->fd_out == -1)
    {
        printf("Error opening file\n");
        exit(1);
    }
}

void apply_redir_list(t_cmd *cmd)
{
    t_redir *redir;

    redir = cmd->redir_list;
    while (redir)
    {
        if (redir->type == INPUT)
            apply_INPUT_redir(cmd, redir);
        else if (redir->type == OUTPUT)
            apply_OUTPUT_redir(cmd, redir);
        else if (redir->type == APPEND)
            cmd->fd_out = open(redir->out_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
        redir = redir->next;
    }
    dup2(cmd->fd_in, STDIN_FILENO);
    dup2(cmd->fd_out, STDOUT_FILENO);
}
void one_cmd_case(t_data *data)
{
    t_cmd *cmd;
    pid_t pid;
    char *path;
    int status;

    cmd = data->cmd_list;
    if(is_a_builtin(cmd) == 1)
        exec_builtin(cmd);
    else
    {
        pid = fork();
        if(pid == 0)
        {
            if(cmd->redir_list)
                apply_redir_list(cmd);
            path = get_path(cmd->array_cmd[0], data->env);
            execve(path, cmd->array_cmd, data->env);
            printf("Comand not found\n");
            exit(127);
        }
        else
        {
            waitpid(pid, &status, 0); //El exit status es una info que se tiene que interpretar con la macro WIFEXITED
            data->exit_status = WEXITSTATUS(status); //WEXITSTATUS es una macro que devuelve el exit status del hijo
        }
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
    basic_parsing(data);
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(OUTPUT, NULL, "out_file.txt"));
    add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "infile1.txt", NULL));
    add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "infile2.txt", NULL));
    if (cmd_list_len(data->cmd_list) == 1)
        one_cmd_case(data);
    else
        multiple_cmd_case(data);
}
