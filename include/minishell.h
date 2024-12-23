/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvapari <alvapari@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 11:40:24 by achacon-          #+#    #+#             */
/*   Updated: 2024/12/23 02:06:16 by alvapari         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft/libft.h"
# include "ft_printf/ft_printf.h"
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
# include <termios.h>
# include <readline/history.h>
# include <readline/readline.h>

# include "global.h"

typedef struct s_data	t_data;
typedef struct s_cmd	t_cmd;
typedef struct s_pipe	t_pipe;
typedef struct s_redir	t_redir;

typedef enum e_quote_status
{
	DOUB_QUOT = 34,
	ONE_QUOT = 39
}	t_quote_status;

typedef enum e_token_type
{
	CMD,
	PIPE,
	REDIR
}	t_token_type;

typedef enum e_redir_type
{
	INPUT,
	OUTPUT,
	APPEND,
	HERE_DOC
}	t_redir_type;

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
	int					index;
	char				*in_name;
	char				*out_name;
	char				*delim;
	t_redir				*next;
	t_data				*data;
}						t_redir;

typedef struct s_parsing
{
	int					count;
	int					how_much;
	int					reject;
	char				flag;
	char				**arr_lexems;
	char				*arr_toks;
	char				**aux_ar_cmds;
	char				**aux_redirs;
	char				***ar_of_ar;
	int					index_arr;
	int					init_index;
	int					end_index;
	int					cnt_new_str;
	int					len;
	char				flag_space;
	t_data				*ptrdata;
}						t_parsing;

typedef struct s_data
{
	char				**env;
	char				*input;
	char				*in_ax;
	char				**array_var;
	int					exit_status;
	t_cmd				*cmd_list;
	int					pipe[2];
	int					here_doc_counter;
	int					parsing_error;
	t_parsing			prs;
	struct termios		original_settings;
}						t_data;

//----------------------BUILTINS----------------------

////// BUILTINS_UTILS
char					*ft_var_name(char *name_value);
char					*ft_var_value(char *name_value);
char					*ft_getenv(char *var_name, char **env, t_data *data);
char					**insert_var(char **array, \
							char *var_name, char *var_value);
char					*remove_brackets(char *var_name);
////// BUILTINS_EXECUTION
int						is_a_builtin(t_cmd *cmd);
void					exec_builtin(t_cmd *cmd);
void					builtin_end(t_data *data, int error);
////// FT_ECHO
int						has_variable(char *input);
char					*get_exp_var(char *s, int i);
void					print_expanded(char *input, char **env);
void					ft_echo(t_cmd *cmd);
////// FT_PWD
void					ft_pwd(t_cmd *cmd);
////// FT_CD
void					ft_cd_home(t_data *data, char *oldpwd, char *pwd);
void					ft_cd(t_cmd *cmd);
void					safe_init_cd(t_cmd *cmd, char **oldpwd, char **pwd);
void					update_env(t_data *data, char **oldpwd, char **pwd);
void					free_dirs(char *oldpwd, char *pwd);
////// FT_ENV
void					ft_env(t_cmd *cmd);
////// FT_EXPORT
void					print_export(char **array_var);
void					ft_export(t_cmd *cmd);
////// FT_UNSET
void					ft_unset(t_cmd *cmd);
void					delete_var(char **array, char *var_name);
////// FT_EXIT
void					ft_exit(t_cmd *cmd);

//----------------------DATA----------------------
void					init_data(t_data *data, char **env);
void					free_data(t_data *data);
void					reboot_data(t_data *data);
void					empty_env(t_data *data);

//----------------------DATA----------------------

//////DATA UTILS
void					update_shlvl(t_data *data);
void					empty_env(t_data *data);
//////DATA
void					init_data(t_data *data, char **env);
void					free_data(t_data *data);
void					reboot_data(t_data *data);
void					free_parsing(t_parsing *prs);

//----------------------EXECUTION----------------------

////// EXECUTION UTILS
void					close_fds(void);
void					exit_process(t_data *data, int exit_status);
void					safe_fork(pid_t *pid, t_data *data);
void					exec_cmd(t_cmd *cmd, t_data *data);
void					safe_dup2(int *fd_in, int *fd_out, t_data *data);
////// EXECUTION
void					execution(t_data *data);
////// MULTIPLE CMD
void					multiple_cmd_case(t_data *data);
void					child(t_cmd *cmd, int *fd_in, \
							int *fd_out, t_data *data);

////// ONE CMD
void					one_cmd_case(t_data *data);
void					one_builtin_case(t_cmd *cmd);
////// PATH
char					*get_path_line(char **env);
char					*get_path(char *command, char **env);

//----------------------LISTS----------------------

//////CMD LIST UTILS (1)
t_cmd					*new_cmd(char **array_cmds, t_data *data);
t_cmd					*add_cmd(t_cmd *cmd_list, t_cmd *new_cmd);
void					free_cmd(t_cmd *cmd);
t_cmd					*free_cmd_list(t_cmd *cmd_list);
int						cmd_list_len(t_cmd *cmd_list);
//////CMD LIST UTILS (2)
t_cmd					*get_last_cmd(t_cmd *cmd_list);
t_cmd					*get_cmd_by_index(t_cmd *cmd_list, int index);
int						is_first_cmd(t_cmd *cmd);
int						is_last_cmd(t_cmd *cmd);
//////PRINT LISTS
void					print_cmd(t_cmd *cmd);
void					print_cmd_list(t_cmd *cmd_list);
void					print_redir(t_redir *redir);
void					print_redir_list(t_redir *redir_list);
char					*redir_type_to_string(t_redir_type type);
//////REDIR LIST UTILS
t_redir					*new_redir(t_redir_type type, \
							char *input, t_data *data);
void					update_index_redir_list(t_redir *redir_list);
void					add_redir(t_cmd *cmd, t_redir *redir);
t_redir					*free_redir_list(t_redir *redir_list);

//----------------------MAIN----------------------

//////MAIN
void					header(void);
void					history(char *input);
int						main(int argc, char **argv, char **env);

//----------------------REDIRS----------------------

//////REDIRS EXECUTION
int						update_fds_redirs(t_cmd *cmd_list);
void					dup_fds_redirs(t_cmd *cmd);
void					apply_last_out_redir(t_cmd *cmd);
void					apply_last_in_redir(t_cmd *cmd);
int						open_and_try_redir(t_redir *redir);

//////HERE DOC REDIR
void					delete_temp_file(t_cmd *cmd, char *temp_file);
void					prompt_loop(int here_doc_fd, t_redir *redir);
char					*temp_file_name(t_cmd *cmd);
void					apply_heredoc_redir(t_cmd *cmd, t_redir *redir);
void					apply_every_heredoc(t_data *data);
//////HERE DOC UTILS
char					*append_char(char *str, char c);
char					*hd_var_name(char *line, int i);
char					*append_var_value(char *expand_line, \
							char *line, int *i, t_data *data);
char					*expand_vars_hd(char *line, t_data *data);

//////INPUT REDIR
void					apply_input_redir(t_cmd *cmd, t_redir *redir);
//////OUTPUT REDIR
void					apply_output_redir(t_cmd *cmd, t_redir *redir);
//////APPEND REDIR
void					apply_append_redir(t_cmd *cmd, t_redir *redir);
//////REDIR UTILS
t_redir					*get_last_in_redir(t_cmd *cmd);
t_redir					*get_last_out_redir(t_cmd *cmd);
void					print_error_openining_file(char *file_name);
int						open_and_try_redir_output(t_redir *redir);
int						open_and_try_redir_append(t_redir *redir);

//----------------------SIGNALS----------------------
//////SIGNALS
void					signals_handler(void);
void					handle_ctrl_c(int signal);
void					handle_backlash(int sign);
void					signals_heredoc(void);
t_data					*get_data(t_data *data);

//////DISABLE PRINTING SIGNALS
void					disable_printing_signals(t_data *data);
void					restore_original_settings(t_data *data);

//----------------------UTILS----------------------

////// UTILS (1)
int						ft_only_spaces(char *input);
int						array_len(char **array);
void					swap(char **a, char **b);
int						ft_strcmp(const char *s1, const char *s2);
void					print_array(char **array);

////// UTILS (2)
void					free_array(char **array);
char					**copy_alloc_array(char **array);
void					sort_strings(char **array, int size);
char					**realloc_elonged_array(char **src_array);
void					free_triple_ptr(char ***ptr);

//----------------------PARSING----------------------

// PARSING-SPLIT
void					ft_start_parsing(char *str, t_parsing *prs, int top);
void					ft_init_parsing_struc(t_parsing *prs);
void					ft_if_single_quote(t_parsing *prs);
void					ft_if_doub_quote(t_parsing *prs);
void					ft_create_arr_lexem(char *str, t_parsing *prs);
void					ft_string_by_string(char *str, t_parsing *prs);
void					ft_split_strings(char *str, t_parsing *prs);
void					ft_if_doub_quote_split_strings(t_parsing *prs, \
							char *str);
void					ft_if_single_quote_split_strings(t_parsing *prs, \
							char *str);
void					ft_create_a_space_lexem(t_parsing *prs);
int						ft_if_lexem_only_single_quotes(char *str, \
							t_parsing *prs);
int						ft_if_lexem_only_doub_quotes(char *str, t_parsing *prs);
void					ft_print_prs_err(char flag, t_parsing *prs);
void					ft_move_counts(t_parsing	*prs);
void					ft_string_by_string_aux(char *str, t_parsing *prs);
void					ft_printf_proofs_split_prs(t_data data);
void					ft_parsing(t_data *data);
char					*new_input(char *s, int len);
int						ft_len(char *s, int k, int i, int add);
int						ft_tell_if_oq(char *str, int index, int i, int flag);
int						ft_is_spc_chr(char str);
char					*ft_new_input(char *s, int len);
void					ft_check_if_csp(char *str, t_parsing *prs);
void					ft_check_if_csp_aux(char *str, t_parsing *prs);
void					ft_realloc_prs_o(t_parsing *prs);
void					ft_realloc_prs_t(t_parsing *prs);
char					*ft_new_input_aux(char *s, char *new_str, int i, \
							int cnt_ns);

// PARSING-TOKEN
void					ft_create_tk_arr(t_parsing *prs);
void					ft_create_tks(t_parsing *prs, int count_2);

// PARSING-SEND
void					ft_start_sending(t_parsing *prs, t_data *data, int i);
void					ft_if_pipe_or_rdr(t_parsing *prs, \
							t_data *data, t_cmd *node);
void					ft_if_not_special_char(t_parsing *prs, \
							t_data *data, t_cmd *node);
void					ft_we_are_doing_proofs(t_parsing *prs);
char					**filter_strings(char **input);
int						is_redirection(const char *str);
int						is_redirection_n(const char *str);
char					**filter_redirections(char **input);
void					ft_create_three_ptr(t_parsing *prs, int pipes);
char					**ft_give_me_arr(char **ar_lexems, int init, int end);
int						ft_tell_me_if_pipes(t_parsing *prs);
void					ft_send_to_cmd_str(t_parsing *prs, \
							t_data *data, t_cmd *node, int i);
void					ft_send_rd(t_parsing *prs, t_data *data, int i);
void					ft_send_if_pipe(t_parsing *prs, int pipes, int i);
void					ft_create_arr_lexem(char *str, t_parsing *prs);
void					ft_create_tks(t_parsing *prs, int count_2);
void					ft_check_toks(t_parsing *prs, int len);
void					ft_print_unexpected(t_parsing *prs, int count, int len);
t_cmd					*new_empty_cmd(t_parsing *prs);

// PARSING-EXPANS
void					ft_if_doub_quote_exp(t_parsing *prs);
void					ft_if_single_quote_exp(t_parsing *prs);
void					ft_start_expas(t_parsing *prs);
void					ft_expan_n_times(t_parsing *prs, int times);
void					ft_dollar_by_dollar(t_parsing *prs, int count);
char					*ft_take_name_var(t_parsing *prs, int cnt_str);
char					*ft_look_for_var_value(char **env,
							const char *var_name);
char					*ft_strjoin_insert(const char *s1, const char *add,
							size_t insrt_indx, size_t del_len);
char					*ft_return_aux(t_parsing *prs, int count);
int						ft_condition(t_parsing *prs, int count);

#endif