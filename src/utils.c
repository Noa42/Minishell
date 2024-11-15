#include "../include/minishell.h"

void print_array(char **array)
{
    int i = 0;
    while (array[i])
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
char **copy_alloc_array(char **array)
{
	int i = 0;
	char **new_array;
	while (array[i])
		i++;
	new_array = malloc(sizeof(char *) * (i + 1));
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}
int array_len(char **array)
{
	int i = 0;
	while (array[i] != NULL)
		i++;
	return (i);
}
void swap(char **a, char **b)
{// Función para intercambiar dos cadenas
    char *temp = *a;
    *a = *b;
    *b = temp;
}

// Función para ordenar las cadenas en orden alfabético (usando bubble sort)
void sort_strings(char **array, int size)
{
    int i, j;
    for (i = 0; i < size - 1; i++) {
        for (j = 0; j < size - i - 1; j++) {
            // Usamos ft_strncmp para comparar las cadenas
            if (ft_strncmp(array[j], array[j + 1], 100) > 0) {
                swap(&array[j], &array[j + 1]);
            }
        }
    }
}
int ft_strcmp(const char *s1, const char *s2)
{
	int i;
	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}
char **realloc_elonged_array(char **src_array)
{
    int len = array_len(src_array);
    // Reservar memoria para un nuevo array con una posición extra (len + 2 para el NULL final)
    char **new_array = malloc((len + 2) * sizeof(char *));
    if (!new_array)
        return NULL;

    // Copiar los elementos del array original al nuevo array
    for (int i = 0; i < len; i++)
    {
        new_array[i] = malloc(strlen(src_array[i]) + 1);
        if (!new_array[i])
        {
            // Liberar memoria en caso de fallo para evitar leaks
            while (--i >= 0)
                free(new_array[i]);
            free(new_array);
            return NULL;
        }
        ft_strlcpy(new_array[i], src_array[i], strlen(src_array[i]) + 1);
    }

    // Asignar el último elemento como NULL para finalizar el array
    new_array[len] = NULL;
    new_array[len + 1] = NULL;
    return (new_array);
}
