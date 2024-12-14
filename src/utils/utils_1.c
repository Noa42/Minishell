#include "../../include/minishell.h"

void	history(char *input)
{
	if (input != NULL && ft_strlen(input) != 0 && ft_only_spaces(input) == 0)
		add_history(input);
}

int	ft_only_spaces(char *input)
{
	int	i;

	i = 0;
	while (input[i] != '\0')
	{
		if (input[i] != ' ' && input[i] != '\t')
			return (0);
		i++;
	}
	return (1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int	array_len(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
		i++;
	return (i);
}

void swap(char **a, char **b)
{
	char	*temp;

	temp = *a;
	*a = *b;
	*b = temp;
}
