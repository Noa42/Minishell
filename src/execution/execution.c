#include "../../include/minishell.h"

void	execution(t_data *data)
{
	g_signal_flag = 1;
	signals_handler();
	//basic_parsing(data);
	// add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(OUTPUT, "out_file2.txt", data));
	// //add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "out_file2.txt"));
	// add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in_file1.txt", data));
	// // add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(INPUT, "in_file2.txt"));
	// // add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(HERE_DOC, "delim1", data));
	// // print_cmd_list(data->cmd_list);
	// // add_redir(get_cmd_by_index(data->cmd_list, 0), new_redir(APPEND, "out_file1.txt", data));
	// if (cmd_list_len(data->cmd_list) >= 2)
	// {
	//     add_redir(get_cmd_by_index(data->cmd_list, 1), new_redir(HERE_DOC, "delim2", data));
	//     add_redir(get_cmd_by_index(data->cmd_list, 1), new_redir(APPEND, "out_file2.txt", data));
	//}
	// if (cmd_list_len(data->cmd_list) >= 3)
	// {
	//     add_redir(get_cmd_by_index(data->cmd_list, 2), new_redir(HERE_DOC, "delim3"));
	//     add_redir(get_cmd_by_index(data->cmd_list, 2), new_redir(OUTPUT, "out_file3.txt"));
	// }
	// if (cmd_list_len(data->cmd_list) >= 4)
	// {
	//     add_redir(get_cmd_by_index(data->cmd_list, 3), new_redir(HERE_DOC, "delim4"));
	//     add_redir(get_cmd_by_index(data->cmd_list, 3), new_redir(OUTPUT, "out_file4.txt"));
	// }
	//print_cmd_list(data->cmd_list);
	apply_every_heredoc(data);
	if (cmd_list_len(data->cmd_list) == 1)
		one_cmd_case(data);
	else
		multiple_cmd_case(data);
}

