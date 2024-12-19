#include "../../include/minishell.h"

char	*append_char(char *str, char c)
{
	size_t	len;
	char	*new_str;

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

char	*append_var_value(char *expand_line, char *line, int *i, char **env)
{
	char	*var_name;
	char	*var_value;
	int		j;

	j = 0;
	var_name = hd_var_name(line, *i);
	var_value = ft_getenv(var_name, env);
	while (var_value[j] != '\0')
	{
		expand_line = append_char(expand_line, var_value[j]);
		j++;
	}
	*i += ft_strlen(var_name) + 1;
	free(var_name);
	free(var_value);
	return (expand_line);
}

char	*expand_vars_hd(char *line, char **env)
{
	int		i;
	char	*expand_line;

	i = 0;
	expand_line = ft_strdup("");
	while (line[i] != '\0')
	{
		if (line[i] == '$' && line[i + 1] != '\0')
			expand_line = append_var_value(expand_line, line, &i, env);
		else
		{
			expand_line = append_char(expand_line, line[i]);
			i++;
		}
	}
	free(line);
	return (expand_line);
}
