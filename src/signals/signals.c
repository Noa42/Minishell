#include "../../include/minishell.h"

void	handle_backlash(int sign)
{
	(void)sign;
	if (g_signal_flag == 1)
		printf("Quit (core dumped) \n");
}

void	handle_ctrl_c(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 1); // el 1 es clear_undo = 1, un modo para que no podamos dehacer lo escrito en la line (no tenemos que implementarlo en el proyecto)
	rl_on_new_line();
	if (g_signal_flag == 0)
		rl_redisplay(); //refresca para que aparezca el prompt inmediatamente
}

void	signals_heredoc(void)
{
	signal(SIGINT, SIG_IGN); // Ctrl+C debe ignorarse.
	signal(SIGQUIT, SIG_IGN); // Ctrl+\ debe ignorarse.
	signal(SIGTSTP, SIG_IGN); // Ctrl+Z debe ignorarse.
}
void	signals_handler(void)
{
	if (g_signal_flag == 0) //Normal
	{
		signal(SIGINT, handle_ctrl_c); // Ctrl+C imprime una nueva entrada en una línea nueva.
		signal(SIGQUIT, SIG_IGN); // Ctrl+\ debe ignorarse.
		signal(SIGTSTP, SIG_IGN); // Ctrl+Z debe ignorarse.
	}
	if (g_signal_flag == 1) //Ejecución
	{
		signal(SIGINT, handle_ctrl_c); // Ctrl+C imprime una nueva entrada en una línea nueva.
		signal(SIGQUIT, handle_backlash); // Ctrl+\ debe ignorarse.
		signal(SIGTSTP, SIG_IGN); // Ctrl+Z debe ignorarse.
	}
	if (g_signal_flag == 2) //Heredoc
	{
		signal(SIGINT, SIG_IGN); // Ctrl+C debe ignorarse.
		signal(SIGQUIT, SIG_IGN); // Ctrl+\ debe ignorarse.
		signal(SIGTSTP, SIG_IGN); // Ctrl+Z debe ignorarse.
	}
}
