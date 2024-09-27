#include "minishell.h"

static t_terminal *init_term(void);

void reader_loop(void)
{
	t_cmd *cmds;
	char *input;
	t_string input_cpy;
	t_terminal *t;

	while (true)
	{
		t = init_term();
		input = readline("> ");
		input_cpy = cstr_to_str(input);
		add_history(input);
		free(input);
		cmds = parse(input_cpy, t);
		exec(cmds, t);
	}
}

static t_terminal *init_term(void)
{
	t_terminal *t;

	t = malloc(sizeof(t_terminal));
	t->last = NULL;
	//AFAZER: libertar
	t->cmds = NULL;
	t->cmds_num = 1;
	return (t);
}

