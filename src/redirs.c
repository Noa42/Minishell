#include "../include/minishell.h"

void apply_INPUT_redir(t_cmd *cmd, t_redir *redir)
{
    cmd->fd_in = open(redir->in_name, O_RDONLY);
    if (cmd->fd_in == -1)
    {
        ft_putstr_fd("Error opening file\n", 2);
        cmd->data->exit_status = 1;
        return ;
    }
}
void apply_OUTPUT_redir(t_cmd *cmd, t_redir *redir)
{
    cmd->fd_out = open(redir->out_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (cmd->fd_out == -1)
    {
        ft_putstr_fd("Error opening file\n", 2);
        cmd->data->exit_status = 1;
        return ;
    }
}

void apply_APPEND_redir(t_cmd *cmd, t_redir *redir)
{
    cmd->fd_out = open(redir->out_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (cmd->fd_out == -1)
    {
        ft_putstr_fd("Error opening file\n", 2);
        cmd->data->exit_status = 1;
        return ;
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
        ft_putstr_fd("Error opening file\n", 2);
        close(here_doc_fd);
        cmd->data->exit_status = 1;
        return ;
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
        ft_putstr_fd("Error opening file\n", 2);
        cmd->data->exit_status = 1;
        return ;
    }
    if (unlink(temp_file) == -1) //borra el archivo pero la info de dentro sigue siendo accesible en teoria (y de hecho funciona xD)
    {
        ft_putstr_fd("Error deleting temp file\n", 2);
        cmd->data->exit_status = 1;
        return ;
    }
    //al final de apply_redir_list se hace el dup2 del fdin que tiene la info del here_doc
}

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