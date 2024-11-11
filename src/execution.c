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
        *fd_out = STDOUT_FILENO; // Si es el último comando, redirige a la salida estándar
    dup2(*fd_in, STDIN_FILENO); // Redirige la entrada estándar a la salida del comando anterior
    dup2(*fd_out, STDOUT_FILENO); // Redirige la salida estándar al fd_out seleccionado
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

    fd_out = STDOUT_FILENO;// Descriptor de archivo para la salida del último comando
    fd_in = STDIN_FILENO; // Descriptor de archivo para la entrada del primer comando
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
    printf("dentro de apply_input_redir\n");
    cmd->fd_in = open(redir->in_name, O_RDONLY);
    if (cmd->fd_in == -1)
    {
        printf("Error opening file\n");
        free_data(cmd->data);
        exit(1);
    }
}
void apply_OUTPUT_redir(t_cmd *cmd, t_redir *redir)
{
    cmd->fd_out = open(redir->out_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (cmd->fd_out == -1)
    {
        printf("Error opening file\n");
        free_data(cmd->data);
        exit(1);
    }
}

void apply_APPEND_redir(t_cmd *cmd, t_redir *redir)
{
    cmd->fd_out = open(redir->out_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (cmd->fd_out == -1)
    {
        printf("Error opening file\n");
        free_data(cmd->data);
        exit(1);
    }
}
void apply_HERE_DOC_redir(t_cmd *cmd, t_redir *redir)
{
    char *line;
    char *temp_file;
    int here_doc_fd;
    int i;

    i = 0;
    temp_file = ".temp_file.txt"; //Usamos un archivo temporal para guardar la info del here_doc
    line = NULL;//por si acaso
    here_doc_fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); //primero abro el archivo solo para escritura, lo creo si no exite, es oculto
    // printf("1.-.heredocfd %i\n", here_doc_fd);
    if(here_doc_fd == -1)
    {
        printf("Error opening file\n");
        free_data(cmd->data);
        exit(1);
    }
    while(1) //Leo las lineas hasta que encuentre el delimitador. va escribiendo lo que recibe readline en el archivo temporal
    {
        line = readline("heredoc> ");
        if(line == NULL)
        {
            printf("warning: here-document at line %i delimited by end-of-file (wanted `%s')\n", i, cmd->redir_list->delim);
            break;
        }
        if(ft_strcmp(line, redir->delim) == 0)
            break;
        write(here_doc_fd, line, ft_strlen(line));
        write(here_doc_fd, "\n", 1);
        free(line);
        i++;
    }
    free(line);//hay que hacerlo otra vez porque cuando se sale del bucle no se libera la ultima linea
    close(here_doc_fd); // lo cerramos para volver a abrirlo pero en modo lectura
    cmd->fd_in = open(temp_file, O_RDONLY);
    if(cmd->fd_in == -1)
    {
        printf("Error opening file\n");
        free_data(cmd->data);
        exit(1);
    }
    if (unlink(temp_file) == -1) //borra el archivo pero la info de dentro sigue siendo accesible en teoria (y de hecho funciona xD)
    {
        printf("Error deleting file\n");
        close(cmd->fd_in);
        free_data(cmd->data);
        exit(1);
    }
    //al final de apply_redir_list se hace el dup2 del fdin que tiene la info del here_doc
}

// void apply_redir_list(t_cmd *cmd) //primera version no sobreescribe bien redirecciones del mismo tipo
// {
//     t_redir *redir;

//     redir = cmd->redir_list;
//     while (redir)
//     {
//         if (redir->type == INPUT)
//             apply_INPUT_redir(cmd, redir);
//         else if (redir->type == OUTPUT)
//             apply_OUTPUT_redir(cmd, redir);
//         else if (redir->type == APPEND)
//             apply_APPEND_redir(cmd, redir);
//         else if(redir->type == HERE_DOC)
//             apply_HERE_DOC_redir(cmd);
//         redir = redir->next;
//     }
//     if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
//     {
//         printf("Error duplicating file descriptor\n");
//         free_data(cmd->data);
//         exit(1);
//     }
//     if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
//     {
//         printf("Error duplicating file descriptor\n");
//         free_data(cmd->data);
//         exit(1);
//     }
// }

void apply_last_out_redir(t_cmd *cmd)
{
    t_redir *redir;
    t_redir *last_out_redir;

    last_out_redir = NULL;
    redir = cmd->redir_list;
    while (redir)
    {
        if (redir->type == OUTPUT || redir->type == APPEND)
            last_out_redir = redir;
        redir = redir->next;
    }
    if (last_out_redir != NULL)
    {
        if (last_out_redir->type == OUTPUT)
            apply_OUTPUT_redir(cmd, last_out_redir);
        else if (last_out_redir->type == APPEND)
            apply_APPEND_redir(cmd, last_out_redir);
    } else
        cmd->fd_out = 1;
}

void apply_last_in_redir(t_cmd *cmd)
{
    t_redir *redir;
    t_redir *last_in_redir;

    last_in_redir = NULL;
    redir = cmd->redir_list;
    while (redir)
    {
        if (redir->type == INPUT || redir->type == HERE_DOC)
            last_in_redir = redir;
        redir = redir->next;
    }
    if (last_in_redir != NULL)
    {
        if (last_in_redir->type == INPUT)
            apply_INPUT_redir(cmd, last_in_redir);
        else if (last_in_redir->type == HERE_DOC)
            apply_HERE_DOC_redir(cmd, last_in_redir);
    } else
        cmd->fd_in = 0;
}

void apply_redir_list(t_cmd *cmd)
{
    apply_last_in_redir(cmd);
    apply_last_out_redir(cmd);
    if (cmd->fd_in != 0)
    {
        if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
        {
            printf("Error duplicating file descriptor\n");
            free_data(cmd->data);
            exit(1);
        }
    }
    if (cmd->fd_out != 1)
    {
        if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
        {
            printf("Error duplicating file descriptor\n");
            free_data(cmd->data);
            exit(1);
        }
    }
}

void one_builtin_case(t_cmd *cmd)
{
    int stdin_copy;
    int stdout_copy;

    stdin_copy = dup(STDIN_FILENO);
    stdout_copy = dup(STDOUT_FILENO);
    apply_redir_list(cmd);
    exec_builtin(cmd);
    close(cmd->fd_in);
    close(cmd->fd_out);
    dup2(stdin_copy, STDIN_FILENO);
    dup2(stdout_copy, STDOUT_FILENO);
}

void one_cmd_case(t_data *data)
{
    t_cmd *cmd;
    pid_t pid;
    char *path;
    int status;

    cmd = data->cmd_list;
    if(is_a_builtin(cmd) == 1)
        one_builtin_case(cmd);
    else
    {
        pid = fork();
        if(pid == 0)
        {
            if(cmd->redir_list)
                apply_redir_list(cmd);
            path = get_path(cmd->array_cmd[0], data->env);
            execve(path, cmd->array_cmd, data->env);
            printf("Comand not found\n");//esto solo ocurre si el execve falla
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
    // add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(OUTPUT, "out_file2.txt"));
    // add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "out_file2.txt"));
    // add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in_file.txt"));
    add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in_file2.txt"));
    add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(HERE_DOC, "delim"));
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in_file1.txt"));
    if (cmd_list_len(data->cmd_list) == 1)
        one_cmd_case(data);
    else
        multiple_cmd_case(data);
}
