#include "../../include/minishell.h"

void	ft_pwd(t_cmd *cmd)
{
	char	*dir;

	dir = malloc(sizeof(char) * 1024);
	if (dir == NULL)
	{
		ft_putstr_fd("Cannot allocate memory\n", 2);
		exit_process(cmd->data, 1);
	}
	if (getcwd(dir, 1024) != NULL)
		printf("%s\n", dir);
	free(dir);
	builtin_end(cmd->data, errno);
}
