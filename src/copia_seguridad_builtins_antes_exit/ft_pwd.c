#include "../include/minishell.h"

void ft_pwd(void)
{
    char *dir;

    dir = malloc(sizeof(char)* 1024);
    getcwd(dir, 1024);
    printf("%s\n", dir);
}