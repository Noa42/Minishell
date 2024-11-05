
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# include <readline/history.h>
# include <readline/readline.h>

typedef struct s_data	t_data;
typedef struct s_cmd	t_cmd;
typedef struct s_pipe	t_pipe;
typedef struct s_redir	t_redir;

typedef enum e_token_type
{
    CMD,
    PIPE,
    REDIR
}   t_token_type;

typedef enum e_redir_type
{
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC
}   t_redir_type;

typedef struct s_cmd
{
	int					index;
	char				**array_cmd;
	int					fd_in;
	int					fd_out;
	t_redir				*redir_list;
	t_data				*data;
	struct s_cmd		*next;
}						t_cmd;

typedef struct s_redir
{
	t_redir_type		type;
	char				*in_name;
	char				*out_name;
	int					fd_in;
	int					fd_out;
	t_redir				*next;
	t_data				*data;
}						t_redir;

typedef struct s_token
{
	int					index;
	t_token_type		token_type;
	struct s_token		*next;
}						t_token;

typedef struct s_data
{
	char				**env;
	char				*input;
	char				**array_input;
	char				**array_var;
	int					exit_status;
	t_token				*token_list;
    t_cmd         	    *cmd_list;
	int					*pipe;
}						t_data;
//LIST_UTILS
void					instrucciones_ejemplo_listas(t_data *data);
void					print_cmd(t_cmd *cmd);
void					print_cmd_list(t_cmd *cmd_list);
void					free_cmd(t_cmd *cmd);
t_cmd					*free_cmd_list(t_cmd *cmd_list);
t_token					*free_token_list(t_token *token_list);
void					print_token_list(t_token *token_list);
int						cmd_list_len(t_cmd *cmd_list);
t_cmd					*new_cmd(char **array_cmds);
void					add_redir(t_cmd *cmd, t_redir *redir);
t_redir					*new_redir(t_redir_type type, char *in_name, char *out_name);
void					print_redir_list(t_redir *redir_list);
char					*redir_type_to_string(t_redir_type type);
t_cmd					*add_cmd(t_cmd *cmd_list, t_cmd *new_cmd);
t_cmd					*get_last_cmd(t_cmd *cmd_list);
t_cmd					*get_cmd_by_index(t_cmd *cmd_list, int index);
char					*token_type_to_string(t_token_type token_type);



//PRUEBAS EJECTUCION
void					prueba_ejecucion(t_data *data);
void					one_cmd_case(t_data *data);
//void					multiple_cmd_case(t_data *data);
// SIGNALS
void					signals(void);
void					handle_ctrl_c(int signal);

// PARSING
void					parsing(t_data *data);

// UTILS
void					print_array(char **array);
void					free_array(char **array);
char					**copy_alloc_array(char **array);
int						array_len(char **array);
void					sort_strings(char **array, int size);
void					swap(char **a, char **b);
int						ft_strcmp(const char *s1, const char *s2);
char					**realloc_elonged_array(char **src_array);

// EXECUTION
void					execution(t_data *data);
void					builtins(t_data *data);

// SRC //BUILTINS
// FT_ECHO
int						has_variable(char *input);
char					*get_exp_var(char *s, int i);
void					print_expanded(char *input, char **env);
void					ft_echo(t_data *data);
// FT_PWD
void					ft_pwd(void);
// FT_CD
void					ft_cd_home(t_data *data, char *oldpwd, char *pwd);
void					ft_cd(t_data *data);
// FT_ENV
void					ft_env(t_data *data);
// FT_EXPORT
void					print_export(char **array_var);
void					ft_export(t_data *data);
// FT_UNSET
void					ft_unset(t_data *data);
void					delete_var(char **array, char *var_name);
// BUILDINS_UTILS
char					*ft_var_name(char *name_value);
char					*ft_var_value(char *name_value);
char					*ft_getenv(char *var_name, char **env);
char					**insert_var(char **array, char *var_name,
							char *var_value);

// PATH
char					*get_path_line(char **env);
char					*get_path(char *command, char **env);

#endif