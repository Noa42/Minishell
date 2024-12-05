#include "../../include/minishell.h"

void	ft_cd_home(t_data *data, char *oldpwd, char *pwd)
{
	char	*home;

	home = ft_getenv("HOME", data->env);
	if (chdir(home) != 0)
	{
		free(oldpwd);
		free(pwd);
		builtin_end(data, errno);
		return ;
	}
	else
	{
		data->env = insert_var(data->env, "OLDPWD", oldpwd);
		data->array_var = insert_var(data->array_var, "OLDPWD", oldpwd);
		getcwd(pwd, 1024);
		data->env = insert_var(data->env, "PWD", pwd);
		data->array_var = insert_var(data->array_var, "PWD", pwd);
		free(home);
		free(pwd);
		free(oldpwd);
		builtin_end(data, errno);
	}
}

void	update_env(t_data *data, char *oldpwd, char *pwd)
{
	data->env = insert_var(data->env, "OLDPWD", oldpwd);
	data->array_var = insert_var(data->array_var, "OLDPWD", oldpwd);
	getcwd(pwd, 1024);
	data->env = insert_var(data->env, "PWD", pwd);
	data->array_var = insert_var(data->array_var, "PWD", pwd);
	free(pwd);
	free(oldpwd);
}

void	safe_init_cd(t_cmd *cmd, char *oldpwd, char *pwd)
{
	if (oldpwd == NULL || pwd == NULL)
	{
		ft_putstr_fd("Cannot allocate memory\n", 2);
		exit_process(cmd->data, 1);
	}
	getcwd(oldpwd, 1024);
}

void	free_dirs(char *oldpwd, char *pwd)
{
	free(oldpwd);
	free(pwd);
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
	safe_init_cd(cmd, oldpwd, pwd);
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
