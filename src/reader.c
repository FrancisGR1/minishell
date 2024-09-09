#include "minishell.h"

void read_loop(void)
{
	char *prompt;

	while (true)
	{
		rl_clear_history();
		ft_putstr_fd( "> ", STDOUT_FILENO);
		prompt = readline(NULL);
	}
}
