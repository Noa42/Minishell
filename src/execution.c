#include "../include/minishell.h"

void exit_process(t_data *data, int exit_status)
{
    free_data(data);
    exit(exit_status);
}

void exec_here_doc(t_cmd *cmd, t_redir *redir, int i) //modificar
{
    char *line;
    char *temp_file;
    int here_doc_fd;

    temp_file = ft_strjoin(".temp_file_", ft_itoa(i)); //Usamos un archivo temporal para guardar la info del here_doc
    temp_file = ft_strjoin(temp_file, ".txt");
    line = NULL;//por si acaso
    here_doc_fd = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); //primero abro el archivo solo para escritura, lo creo si no exite, es oculto
    if(here_doc_fd == -1)
    {
        ft_putstr_fd("Error opening file\n", 2);
        close(here_doc_fd);
        cmd->data->exit_status = 1;
        return;
    }
    printf("Delim: %s\n", redir->delim);
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
    }
    free(line);//hay que hacerlo otra vez porque cuando se sale del bucle no se libera la ultima linea
    close(here_doc_fd); // lo cerramos para volver a abrirlo pero en modo lectura
    cmd->fd_in = open(temp_file, O_RDONLY);
    if(cmd->fd_in == -1)
    {
        ft_putstr_fd("Error opening file\n", 2);
        cmd->data->exit_status = 1;
        free(temp_file);
        return ;
    }
    if (unlink(temp_file) == -1) //borra el archivo pero la info de dentro sigue siendo accesible en teoria (y de hecho funciona xD)
    {
        ft_putstr_fd("Error deleting temp file\n", 2);
        cmd->data->exit_status = 1;
        free(temp_file);
        return ;
    }
    free(temp_file);
    return ;
}

void here_docs_first(t_cmd *cmd_list) //creo que el array me sobra y por tanto el here_doc_counter tambien
{
    t_cmd *cmd;
    int i;

    cmd = cmd_list;
    i = 0;
    while(cmd)
    {
        if (get_last_in_redir(cmd)->type == HERE_DOC)
        {
            exec_here_doc(cmd, get_last_in_redir(cmd), i);
            i++;
        }
        cmd = cmd->next;
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

    here_docs_first(data->cmd_list);

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

void new_apply_redir_list(t_cmd *cmd) //esta funciona en multiplecmd con varios heredocs
{
    if (cmd->redir_list == NULL)
        return ;
    if(get_last_in_redir(cmd)->type != HERE_DOC)
        apply_last_in_redir(cmd);
    apply_last_out_redir(cmd);
    if (cmd->fd_in == -1 || cmd->fd_out == -1)
        return;
    if (cmd->fd_in != 0)
    {
        if (dup2(cmd->fd_in, STDIN_FILENO) == -1)
        {
            ft_printf("Error duplicating file descriptor\n", 2);
            cmd->data->exit_status = 1;
            return;
        }
    }
    if (cmd->fd_out != 1)
    {
        if (dup2(cmd->fd_out, STDOUT_FILENO) == -1)
        {
            ft_printf("Error duplicating file descriptor\n", 2);
            cmd->data->exit_status = 1;
            return;
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

    new_apply_redir_list(cmd); ///AÑADIDO AQUI FUNCIONA CON TODAS EXCEPTO HEREDOC

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
    basic_parsing(data);
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "out_file2.txt"));
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "out_file2.txt"));
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in_file1.txt"));
    //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in_file2.txt"));
    add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(HERE_DOC, "delim1"));
    add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(OUTPUT, "out_file1.txt"));
    if (cmd_list_len(data->cmd_list) == 2)
    {
        add_redir(get_cmd_by_index(data->cmd_list, 1), new_redir(INPUT, "in_file1.txt"));
        add_redir(get_cmd_by_index(data->cmd_list, 1), new_redir(HERE_DOC, "delim2"));
        add_redir(get_cmd_by_index(data->cmd_list, 1), new_redir(APPEND, "out_file2.txt"));
    }
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
    apply_redir_list(cmd);
    if (cmd->fd_in == -1 || cmd->fd_out == -1)
    {
        close(stdin_copy);
        close(stdout_copy);
        return;
    }
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
            apply_redir_list(cmd);
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
