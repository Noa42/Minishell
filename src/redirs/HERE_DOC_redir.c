#include "../../include/minishell.h"

char *temp_file_name(t_cmd *cmd)
{
	char	*temp_file;
	char	*temp_join;
	char	*counter;

	counter = ft_itoa(cmd->data->here_doc_counter); // Guardamos el resultado del ft_itoa para poder hacer free
	temp_join = ft_strjoin(".temp_file_", counter); //Guardamos el resultado del ft_strjoin para poder hacer free
	cmd->data->here_doc_counter++;
	temp_file = ft_strjoin(temp_join, ".txt");
	free(temp_join);
	free(counter);
	return (temp_file);
}

// void	add_char(char *str, char *pos, char c)
//  {
// 	int		i;
// 	char	*new_str;

// 	if(str == NULL)
// 	{
// 		str = malloc(2*sizeof(char));
// 		str[0] = c;
// 		str[1] = '\0';
// 		return ;
// 	}
// 	i = 0;
// 	while (&str[i] != pos)
// 		i++;
// 	new_str = malloc(i + 2);
// 	new_str[i] = c;
// 	new_str[i + 1] = '\0';
// 	free(str);
// 	str = new_str;
//  }

char	*append_char(char *str, char c)
{
	size_t	len;
	char	*new_str;

	if (str == NULL)
	{
		str = malloc(2 * sizeof(char));
		str[0] = c;
		str[1] = '\0';
		return (str);
	}
	len = ft_strlen(str);
	new_str = malloc(len + 2);
	if (!new_str)
		return (NULL);
	ft_strlcpy(new_str, str, len + 1);
	new_str[len] = c;
	new_str[len + 1] = '\0';
	free(str);
	return (new_str);
}

char	*hd_var_name(char *line, int i)
{
	int		j;
	char	*var_name;

	j = i + 1;
	while (line[j] != ' ' && line[j] != '\0')
		j++;
	var_name = ft_substr(line, i + 1, j - i - 1);
	return (var_name);
}

char	*expand_vars_hd(char *line, char **env)
{
	int		i;
	char	*expand_line;
	char	*var_name;
	char	*var_value;
	int		j;

	j = 0;
	i = 0;
	expand_line = NULL;
	var_name = NULL;
	var_value = NULL;
	while (line[i] != '\0')
	{
		if (line[i] == '$' && line[i + 1] != '\0')
		{
			j = 0;
			if (var_name != NULL)
				free(var_name);
			if (var_value != NULL)
				free(var_value);
			var_name = hd_var_name(line, i);
			var_value = ft_getenv(var_name, env);
			if (var_value == NULL)
				expand_line = append_char(expand_line, ' ');
			else
			{
				while (var_value[j] != '\0')
				{
					expand_line = append_char(expand_line, var_value[j]);
					j++;
				}
			}
			i += ft_strlen(var_name) + 1;
		}
		else
		{
			expand_line = append_char(expand_line, line[i]);
			i++;
		}
	}
	free(line);
	if (var_name != NULL)
		free(var_name);
	if (var_value != NULL)
		free(var_value);
	return (expand_line);
}

// void	prompt_loop(int here_doc_fd, t_redir *redir) //Funcion original
// {
// 	char	*line;
// 	int		i;

// 	i = 0;
// 	while (1) //Leo las lineas hasta que encuentre el delimitador. va escribiendo lo que recibe readline en el archivo temporal
// 	{
// 		line = readline("heredoc> ");
// 		if (line == NULL)
// 		{
// 			printf("warning: here-document at line %i 
// 				delimited by end-of-file (wanted `%s')\n", i, redir->delim);
// 			break ;
// 		}
// 		i++;
// 		if (ft_strcmp(line, redir->delim) == 0)
// 			break ;
// 		write(here_doc_fd, line, ft_strlen(line));
// 		write(here_doc_fd, "\n", 1);
// 		free(line);
// 	}
// 	free(line);
// }

void	prompt_loop(int here_doc_fd, t_redir *redir)
{
	char	*line;
	int		i;

	i = 0;
	while (1) //Leo las lineas hasta que encuentre el delimitador. va escribiendo lo que recibe readline en el archivo temporal
	{
		line = readline("heredoc> ");
		if (line == NULL)
		{
			printf("warning: here-document at line %i \
				delimited by end-of-file (wanted `%s')\n", i, redir->delim);
			break ;
		}
		i++;
		if (has_variable(line))
			line = expand_vars_hd(line, redir->data->env);
		if (ft_strcmp(line, redir->delim) == 0)
			break ;
		write(here_doc_fd, line, ft_strlen(line));
		write(here_doc_fd, "\n", 1);
		free(line);
	}
	free(line);
}
void	delete_temp_file(t_cmd *cmd, char *temp_file)
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

void	apply_HERE_DOC_redir(t_cmd *cmd, t_redir *redir)
{
	char	*temp_file;

	temp_file = temp_file_name(cmd);
	cmd->fd_in = \
	open(temp_file, O_WRONLY | O_CREAT | O_TRUNC, 0644); //primero abro el archivo solo para escritura, lo creo si no exite, es oculto
	if (cmd->fd_in == -1)
	{
		ft_putstr_fd("Error opening file\n", 2);
		free(temp_file);
		cmd->data->exit_status = 1;
		return ;
	}
	prompt_loop(cmd->fd_in, redir);
	cmd->fd_in = open(temp_file, O_RDONLY);
	if (cmd->fd_in == -1)
	{
		ft_putstr_fd("Error opening file\n", 2);
		cmd->data->exit_status = 1;
		free(temp_file);
		return ;
	}
	delete_temp_file(cmd, temp_file);
	return ;
}
