/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sending_3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achacon- <achacon-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 02:35:25 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/20 16:35:07 by achacon-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

char	**ft_give_me_arr(char **ar_lexems, int init, int end)
{
	int		len;
	int		count;
	char	**result;

	count = 0;
	len = end - init;
	result = malloc(sizeof(char *) * (len + 1));
	if (!result)
	{
		printf("Array has not been created (Error)");
		exit(0);
	}
	while (count < len)
	{
		result[count] = ft_strdup(ar_lexems[init + count]);
		if (!result[count])
		{
			printf("String has not been created (Error)");
			exit(0);
		}
		count++;
	}
	result[count] = NULL;
	return (result);
}

// Función principal: filtra el array de strings
// Contar cuántas strings NO son redirecciones ni sus archivos
// Saltar también la siguiente string
char	**filter_strings(char **input)
{
	int		count;
	int		i;
	int		j;
	int		k;
	char	**result;

	count = 0;
	i = 0;
	j = 0;
	k = 0;
	while (input[i] != NULL)
	{
		if (is_redirection(input[i]))
			i++;
		else
			count++;
		i++;
	}
	if(count == 0)
		return (NULL);
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (input[i] != NULL)
	{
		if (is_redirection(input[i]))
			i = i + 2;
		else
		{
			result[j] = ft_strdup(input[i]);
			if (!result[j])
			{
				while (k < j)
				{
					free(result[k]);
					k++;
				}
				free(result);
				return (NULL);
			}
			j++;
			i++;
		}
	}
	result[j] = NULL;
	return (result);
}

char	**filter_redirections(char **input)
{
	int		count;
	int		i;
	int		j;
	char	**result;

	count = 0;
	i = 0;
	j = 0;
	while (input && input[i] != NULL)
	{
		if (is_redirection_n(input[i]))
		{
			count += 2;
			i += 2;
		}
		else
			i++;
	}
	result = malloc(sizeof(char *) * (count + 1));
	if (!result)
		return (NULL);
	i = 0;
	while (input[i] != NULL)
	{
		if (is_redirection_n(input[i]))
		{
			result[j] = ft_strdup(input[i]);
			if (!result[j])
				return (NULL);
			j++;
			i++;
			result[j] = ft_strdup(input[i]);
			if (!result[j])
				return (NULL);
			j++;
			i++;
		}
		else
			i++;
	}
	result[j] = NULL;
	return (result);
}
