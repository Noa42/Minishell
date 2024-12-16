/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sending_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:33:21 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/17 00:45:35 by alvapari         ###   ########.fr       */
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
    int i;
    int flag;
    
    flag = 0;
    i = 0;
    while(prs->arr_lexems[i] != NULL)
    {
        if((ft_strlen(prs->arr_lexems[i]) == 1 && prs->arr_lexems[i][0] == '<')
            || (ft_strlen(prs->arr_lexems[i]) == 1 && prs->arr_lexems[i][0] == '>')
            || (ft_strlen(prs->arr_lexems[i]) == 2 && prs->arr_lexems[i][0] == '<'
            && prs->arr_lexems[i][1] == '<') || (ft_strlen(prs->arr_lexems[i])
            == 2 && prs->arr_lexems[i][0] == '>' && prs->arr_lexems[i][1] == '>')
            || (ft_strlen(prs->arr_lexems[i]) == 1 && prs->arr_lexems[i][0] == '|'))
        {
            ft_if_pipe_or_rdr(prs, data, node);
            printf("YA HAREMOS WHATEVER\n");
            flag ++;
        }
        i++;
    }    
    if (flag == 0)
    {
        node = new_cmd(prs->arr_lexems, data);
        data->cmd_list = add_cmd(data->cmd_list, node);
        print_cmd_list(data->cmd_list);
    }        
}

void    ft_if_pipe_or_rdr(t_parsing *prs, t_data *data, t_cmd *node)
{
    
}


/*{   
    t_cmd   *node;
    int i;
    
    i = 0;
    node = new_cmd(prs->arr_lexems, data);
    data->cmd_list = add_cmd(data->cmd_list, node);
    while(prs->arr_lexems[i] != NULL)
    {
        if(ft_strlen(prs->arr_lexems[i]) == 1 && prs->arr_lexems[i][0] == '<')
        {
            add_redir(get_last_cmd(data->cmd_list), 
            new_redir(INPUT, prs->arr_lexems[i + 1] , data));
        }
        if(ft_strlen(prs->arr_lexems[i]) == 1 && prs->arr_lexems[i][0] == '>')
        {
            add_redir(get_last_cmd(data->cmd_list), 
            new_redir(OUTPUT, prs->arr_lexems[i + 1] , data));
        }
        if(ft_strlen(prs->arr_lexems[i]) == 2 && prs->arr_lexems[i][0] == '<'
            && prs->arr_lexems[i][1] == '<')
        {    
            add_redir(get_last_cmd(data->cmd_list), 
            new_redir(HERE_DOC, prs->arr_lexems[i + 1] , data));
        }    
        if(ft_strlen(prs->arr_lexems[i]) == 2 && prs->arr_lexems[i][0] == '>'
            && prs->arr_lexems[i][1] == '>')
        {  
            add_redir(get_last_cmd(data->cmd_list), 
            new_redir(APPEND, prs->arr_lexems[i + 1] , data));
        }          
        i++;
    } 
    print_cmd_list(data->cmd_list);
}

PROBLEMAS:
 - El orden de los archivos después de caracter especial !!!
 - ¿Necesario comprobar en la función de arriba ese i + 1? Posiblemente ya me lo cargue
 en syntax error  y deje por tanto de ser necesario!!
 - Esplitear de array de strings a array de strings hasta carácter especial
 -*/ 
 

 /*PROBAR:   
 #include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Función auxiliar para calcular la longitud de una cadena
size_t ft_strlen(const char *str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

// Función para comprobar si una cadena es un operador de redirección
int is_redirection(const char *str) {
    if (!str)
        return 0;
    size_t len = ft_strlen(str);
    return (len == 1 && (str[0] == '<' || str[0] == '>')) ||
           (len == 2 && ((str[0] == '<' && str[1] == '<') || (str[0] == '>' && str[1] == '>')));
}

// Función principal: filtra el array de strings
char **filter_strings(char **input) {
    // Contar cuántas strings no son redirecciones ni sus asociadas
    int count = 0;
    for (int i = 0; input[i] != NULL; i++) {
        if (is_redirection(input[i])) {
            i++; // Saltar también la siguiente string
        } else {
            count++;
        }
    }

    // Reservar memoria para el nuevo array
    char **result = malloc((count + 1) * sizeof(char *));
    if (!result)
        return NULL;

    // Llenar el nuevo array con las strings que no sean redirecciones ni sus asociadas
    int j = 0;
    for (int i = 0; input[i] != NULL; i++) {
        if (is_redirection(input[i])) {
            i++; // Saltar también la siguiente string
        } else {
            result[j] = strdup(input[i]); // Copiar la cadena al nuevo array
            if (!result[j]) {
                // Liberar memoria en caso de error
                for (int k = 0; k < j; k++)
                    free(result[k]);
                free(result);
                return NULL;
            }
            j++;
        }
    }
    result[j] = NULL; // Terminar el array con NULL
    return result;
}

// Función para liberar memoria de un array de strings
void free_strings(char **strings) {
    if (!strings)
        return;
    for (int i = 0; strings[i] != NULL; i++)
        free(strings[i]);
    free(strings);
}

// Prueba de la función
int main() {
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