/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sending_1.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 19:33:21 by alvapari          #+#    #+#             */
/*   Updated: 2024/12/16 23:59:11 by alvapari         ###   ########.fr       */
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
 