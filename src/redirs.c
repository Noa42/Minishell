#include "../include/minishell.h"

char *temp_file_name(t_cmd *cmd)
{
    char *temp_file;
    char *temp_join;
    char *counter;

    counter = ft_itoa(cmd->data->here_doc_counter); // Guardamos el resultado del ft_itoa para poder hacer free
    temp_join = ft_strjoin(".temp_file_", counter); //Guardamos el resultado del ft_strjoin para poder hacer free
    cmd->data->here_doc_counter++;
    temp_file = ft_strjoin(temp_join, ".txt");
    free(temp_join);
    free(counter);
    return (temp_file);
}

void prompt_loop(int here_doc_fd, t_redir *redir)
{
    char *line;
    int i;

    i = 0;
    while(1) //Leo las lineas hasta que encuentre el delimitador. va escribiendo lo que recibe readline en el archivo temporal
    {
        line = readline("heredoc> ");
        if(line == NULL)
        {
            printf("warning: here-document at line %i delimited by end-of-file (wanted `%s')\n", i, redir->delim);
            break;
        }
        i++;
        if(ft_strcmp(line, redir->delim) == 0)
            break;
        write(here_doc_fd, line, ft_strlen(line));
        write(here_doc_fd, "\n", 1);
        free(line);
    }
    free(line);
}
void delete_temp_file(t_cmd *cmd, char *temp_file)
{
    if (unlink(temp_file) == -1) //borra el archivo pero la info de dentro sigue siendo accesible en teoria (y de hecho funciona xD)
    {
        ft_putstr_fd("Error deleting temp file\n", 2);
        cmd->data->exit_status = 1;
        free(temp_file);
        return ;
    }
    free(temp_file);
}

void apply_HERE_DOC_redir(t_cmd *cmd, t_redir *redir)
{
    char *temp_file;

    temp_file = temp_file_name(cmd);
    cmd->fd_in = open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); //primero abro el archivo solo para escritura, lo creo si no exite, es oculto
    if(cmd->fd_in == -1)
    {
        ft_putstr_fd("Error opening file\n", 2);
        free(temp_file);
        cmd->data->exit_status = 1;
        return;
    }
    prompt_loop(cmd->fd_in, redir);
    cmd->fd_in = open(temp_file, O_RDONLY);
    if(cmd->fd_in == -1)
    {
        ft_putstr_fd("Error opening file\n", 2);
        cmd->data->exit_status = 1;
        free(temp_file);
        return ;
    }
    delete_temp_file(cmd, temp_file);
    return ;
}

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

t_redir *get_last_out_redir(t_cmd *cmd)
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
    return (last_out_redir);
}

void apply_last_out_redir(t_cmd *cmd)
{
    t_redir *last_out_redir;

    last_out_redir = get_last_out_redir(cmd);
    if (last_out_redir != NULL)
    {
        if (last_out_redir->type == OUTPUT)
            apply_OUTPUT_redir(cmd, last_out_redir);
        else if (last_out_redir->type == APPEND)
            apply_APPEND_redir(cmd, last_out_redir);
    } else
        cmd->fd_out = 1;
}

t_redir *get_last_in_redir(t_cmd *cmd)
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
    return (last_in_redir);
}

void apply_last_in_redir(t_cmd *cmd)
{
    t_redir *last_in_redir;

    last_in_redir = get_last_in_redir(cmd);
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
    if (cmd->redir_list == NULL)
        return ;
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

void update_fds_redirs(t_cmd *cmd_list) //esto es para uno o varios comandos
{
    t_cmd *cmd;

    cmd = cmd_list;
    while(cmd)
    {
        if (cmd->redir_list != NULL)
        {
            apply_last_in_redir(cmd);
            apply_last_out_redir(cmd);
        }
        cmd = cmd->next;
    }
}

void dup_fds_redirs(t_cmd *cmd) //Esto es para un solo comando
{
    // if (cmd->fd_in == -1 || cmd->fd_out == -1)
    //     return;
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