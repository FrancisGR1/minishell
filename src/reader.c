#include "minishell.h"

void reader_loop(void)
{
	char *cmd;

	while (true)
	{
		cmd = readline("> ");
		parse(cmd);
		//exec(cmd);
		add_history(cmd);
	}
}
