#include "minishell.h"

void reader_loop(void)
{
	t_string *cmd;
	char *input;
	t_string input_cpy;

	while (true)
	{
		input = readline("> ");
		input_cpy = cstr_to_str(input);
		cmd = parse(input_cpy);
		add_history(input);
		free(input);
		exec(cmd);
	}
}
