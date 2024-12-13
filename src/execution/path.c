
#include "../../include/minishell.h"

char	*get_path_line(char **env)
{
	char	*path_line;
	int		i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			path_line = env[i];
		i++;
	}
	if (path_line == NULL)
		return (NULL);
	path_line = path_line + 5;
	return (path_line);
}
char	*search_path(char *command, char *path_line)
{
	char	**path_array;
	char	*temp;
	char	*path_final;
	int		i;

	i = 0;
	path_array = ft_split(path_line, ':');
	while (path_array[i])
	{
		temp = ft_strjoin(path_array[i], "/");
		path_final = ft_strjoin(temp, command);
		free(temp);
		if (access(path_final, F_OK) == 0)
		{
			free_array(path_array);
			return (path_final);
		}
		free(path_final);
		i++;
	}
	free_array(path_array);
	return (NULL);
}

char	*get_path(char *command, char **env)
{
	char	*path_line;
	char	*path;
	if (ft_strchr(command, '/') != NULL)
	{
		if (access(command, F_OK) == 0)
			return (command);
	}
	path_line = get_path_line(env);
	if (path_line == NULL)
		return (NULL);
	path = search_path(command, path_line);
	return (path);// sucio intentar cambiar si tengo tiempo
}
