/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sending_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:33:21 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/17 23:06:49 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

//Aquí vamos a empezar a usar las funciones de listas, así que conviene 
//que esta función tenda prs y data, ya que las funciones de listas que
//usamos usan data y necesitan nutrirse de una array de lexemas que está
//en prs

void    ft_start_sending(t_parsing *prs, t_data *data)
{
    t_cmd   *node;
    int     i;
    int     flag;
    char    **array;
    
    flag = 0;
    node = NULL;
    i = 0;
    while(prs->arr_lexems[i] != NULL && flag == 0)
    {
        if((ft_strlen(prs->arr_lexems[i]) == 1 && prs->arr_lexems[i][0] == '<')
            || (ft_strlen(prs->arr_lexems[i]) == 1 && prs->arr_lexems[i][0] == '>')
            || (ft_strlen(prs->arr_lexems[i]) == 2 && prs->arr_lexems[i][0] == '<'
            && prs->arr_lexems[i][1] == '<') || (ft_strlen(prs->arr_lexems[i])
            == 2 && prs->arr_lexems[i][0] == '>' && prs->arr_lexems[i][1] == '>')
            || (ft_strlen(prs->arr_lexems[i]) == 1 && prs->arr_lexems[i][0] == '|'))
        {
            ft_if_pipe_or_rdr(prs, data, node);
            flag = 1;
        }
        i++;
    }
    if(flag == 0)
    {
        array = copy_alloc_array(prs->arr_lexems);
        node = new_cmd(array, data);
        data->cmd_list = add_cmd(data->cmd_list, node);
    }
    print_cmd_list(data->cmd_list);
}

void    ft_if_pipe_or_rdr(t_parsing *prs, t_data *data, t_cmd *node)
{
    char    **aux_ar_cmds;
    char    **aux_redirs;
    int i;

    i = 0;
    aux_ar_cmds = filter_strings(prs->arr_lexems);
    aux_redirs = filter_redirections(prs->arr_lexems);
    node = new_cmd(copy_alloc_array(aux_ar_cmds), data);
    data->cmd_list = add_cmd(data->cmd_list, node);
    while(aux_redirs[i] != NULL)
    {
        if(ft_strlen(aux_redirs[i]) == 1 && aux_redirs[i][0] == '<')
        {
            add_redir(get_last_cmd(data->cmd_list), 
            new_redir(INPUT, ft_strdup(aux_redirs[i + 1]), data));
        }
        if(ft_strlen(aux_redirs[i]) == 1 && aux_redirs[i][0] == '>')
        {
            add_redir(get_last_cmd(data->cmd_list), 
            new_redir(OUTPUT, ft_strdup(aux_redirs[i + 1]), data));
        }
        if(ft_strlen(aux_redirs[i]) == 2 && aux_redirs[i][0] == '<'
            && aux_redirs[i][1] == '<')
        {    
            add_redir(get_last_cmd(data->cmd_list), 
            new_redir(HERE_DOC, ft_strdup(aux_redirs[i + 1]), data));
        }    
        if(ft_strlen(aux_redirs[i]) == 2 && aux_redirs[i][0] == '>'
            && aux_redirs[i][1] == '>')
        {  
            add_redir(get_last_cmd(data->cmd_list), 
            new_redir(APPEND, ft_strdup(aux_redirs[i + 1]), data));
        }          
        i++;
    }
    free_array(aux_ar_cmds);
    free_array(aux_redirs);
    print_cmd_list(data->cmd_list);
}
/*
PROBLEMAS:
 - El orden de los archivos después de caracter especial !!!
 - ¿Necesario comprobar en la función de arriba ese i + 1? Posiblemente ya me lo cargue
 en syntax error  y deje por tanto de ser necesario!!
 - Esplitear de array de strings a array de strings hasta carácter especial
 -*/ 



// Función auxiliar para calcular la longitud de una cadena
size_t ft_strlen(const char *str) 
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

// Función para comprobar si una cadena es un operador de redirección
int is_redirection(const char *str) 
{
    if (!str)
        return 0;
    size_t len = ft_strlen(str);
    return (len == 1 && (str[0] == '<' || str[0] == '>')) ||
           (len == 2 && ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>')));
}

int is_redirection_n(const char *str) 
{
    size_t len;

    if (!str)
        return 0;
    len = ft_strlen(str);
    if ((len == 1 && (str[0] == '<' || str[0] == '>')) ||
        (len == 2 && ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>'))))
        return 1;
    return 0;
}


// Función principal: filtra el array de strings
// Contar cuántas strings no son redirecciones ni sus asociadas
// Saltar también la siguiente string
char **filter_strings(char **input) 
{
    int count;
    int i;
    int j;
    int k;
    char    **result;
     
    count = 0;
    i = 0;
    j = 0;
    k = 0; 
    while(input[i] != NULL) 
    {
        if (is_redirection(input[i])) 
            i++; 
        else 
            count++;
        i++;    
    }
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


// Función principal: filtra solo las redirecciones y sus asociadas
char **filter_redirections(char **input) 
{
    int count;
    int i;
    int j;
    int k;
    char **result;

    count = 0;
    i = 0;
    j = 0;
    k = 0;

    // Contar cuántas strings son redirecciones y sus asociadas
    while (input[i] != NULL) 
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
        else
            i++;
    }
    result[j] = NULL;
    return (result);
}


// Prueba de la función
/*int main() {
    // Array de strings de ejemplo
    char *input[] = {
        "CMD", "ARG1", "ARG2", "<", "FILE1",
        "ARG3", "ARG4", "<<", "DELM", ">", 
        "FILE2", ">>", "FILE3", "ARG5", NULL
    };

    // Filtrar las strings
    char **filtered = filter_strings(input);

    // Imprimir el resultado
    if (filtered) {
        for (int i = 0; filtered[i] != NULL; i++) {
            printf("String número %d es: %s\n", i, filtered[i]);
        }
        free_strings(filtered); // Liberar la memoria del array filtrado
    }

    return 0;
}
*/