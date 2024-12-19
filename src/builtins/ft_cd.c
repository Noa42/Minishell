#include "../../include/minishell.h"

void	free_dirs(char *oldpwd, char *pwd)
{
	if (oldpwd != NULL)
		free(oldpwd);
	if (pwd != NULL)
		free(pwd);
}

void	update_env(t_data *data, char *oldpwd, char *pwd)
{
	data->env = insert_var(data->env, "OLDPWD", oldpwd);
	data->array_var = insert_var(data->array_var, "OLDPWD", oldpwd);
	getcwd(pwd, 1024);
	data->env = insert_var(data->env, "PWD", pwd);
	data->array_var = insert_var(data->array_var, "PWD", pwd);
	if (ft_strcmp(oldpwd, "NULL") == 0)
	{
		free_dirs(oldpwd, pwd);
		builtin_end(data, 0);
	}
	else
	{
		free_dirs(oldpwd, pwd);
		builtin_end(data, errno);
	}
}

void	ft_cd_home(t_data *data, char *oldpwd, char *pwd)
{
	char	*home;

	home = ft_getenv("HOME", data->env);
	if (chdir(home) != 0)
	{
		free_dirs(oldpwd, pwd);
		free(home);
		builtin_end(data, errno);
		return ;
	}
	else
	{
		free(home);
		update_env(data, oldpwd, pwd);
	}
}

void	safe_init_cd(t_cmd *cmd, char **oldpwd, char **pwd)
{

	if (oldpwd == NULL || pwd == NULL)
	{
		ft_putstr_fd("Cannot allocate memory\n", 2);
		exit_process(cmd->data, 1);
	}
	if (getcwd(*oldpwd, 1024) == NULL)
	{
		free(*oldpwd);
		*oldpwd = NULL;
		*oldpwd = ft_strdup("NULL");
	}
}


void	ft_cd(t_cmd *cmd)
{
	char	*oldpwd;
	char	*pwd;

	if (array_len(cmd->array_cmd) > 2)
	{
		builtin_end(cmd->data, E2BIG); //E2BIG = Argument list too long
		return ;
	}
	oldpwd = malloc(sizeof(char) * 1024);
	pwd = malloc(sizeof(char) * 1024);
	safe_init_cd(cmd, &oldpwd, &pwd);
	if (cmd->array_cmd[1] == NULL || ft_strcmp(cmd->array_cmd[1], "~") == 0)
	{
		ft_cd_home(cmd->data, oldpwd, pwd);
		return ;
	}
	if (chdir(cmd->array_cmd[1]) != 0)
	{
		free_dirs(oldpwd, pwd);
		builtin_end(cmd->data, errno);
		return ;
	}
	else
		update_env(cmd->data, oldpwd, pwd);
}
