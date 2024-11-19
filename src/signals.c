#include "../include/minishell.h"

void signals(void)
{
	signal(SIGINT, handle_ctrl_c); // Ctrl+C imprime una nueva entrada en una línea nueva.
	signal(SIGQUIT, SIG_IGN); // Ctrl+\ debe ignorarse.
 	signal(SIGTSTP, SIG_IGN); // Ctrl+Z debe ignorarse.
    //Ctrl+D debe cerrar el programa. Pero esto no lo hacemos con las señales porque cuando pulsamos ctrl+d es como si por readline le metieramos Null y entonces el bucle principal del main se cierra.
}

void	handle_ctrl_c(int signal)
{
	(void)signal;
	write(1, "\n", 1);
	rl_replace_line("", 1); // el 1 es clear_undo = 1, un modo para que no podamos dehacer lo escrito en la line (no tenemos que implementarlo en el proyecto)
	rl_on_new_line();
	rl_redisplay(); //refresca para que aparezca el prompt inmediatamente
}
