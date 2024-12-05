#include "../../include/minishell.h"

void	print_array(char **array)
{
	int i = 0;
	while	(array[i])
	{
		printf("[%i]: %s\n", i, array[i]);
		i++;
	}
}
void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i] != NULL)
	{
		free(array[i]);
		i++;
	}
	free(array);
	array = NULL;
}
char	**copy_alloc_array(char **array)
{
	int i;
	char **new_array;

	i = 0;
	new_array = malloc(sizeof(char *) * (array_len(array) + 1));
	if (!new_array)
	{
		return (NULL);
	}
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

// Función para ordenar las cadenas en orden alfabético (usando bubble sort)
void	sort_strings(char **array, int size)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (i < size - 1)
	{
		j = 0;
		while (j < size - i - 1)
		{
			if (ft_strncmp(array[j], array[j + 1], 100) > 0)
			{
				swap(&array[j], &array[j + 1]);
			}
			j++;
		}
		i++;
	}
}

char **realloc_elonged_array(char **src_array)
{
	int		len;
	char	**new_array;
	int		i;

	i = 0;
	len = array_len(src_array);
	new_array = malloc((len + 2) * sizeof(char *));
	if (!new_array)
		return (NULL);
	while (i < len)
	{
		new_array[i] = malloc(strlen(src_array[i]) + 1);
		if (!new_array[i])
		{
			while (--i >= 0)
				free(new_array[i]);
			free(new_array);
			return (NULL);
		}
		ft_strlcpy(new_array[i], src_array[i], strlen(src_array[i]) + 1);
		i++;
	}
	new_array[len] = NULL;
	new_array[len + 1] = NULL;
	return (new_array);
}
