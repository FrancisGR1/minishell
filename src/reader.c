#include "minishell.h"

static t_terminal *init_term(void);
static void reset_term(t_terminal **t);

void reader_loop(void)
{
	t_cmd *cmds;
	char *input;
	t_string input_cpy;
	t_terminal *t;

	t = init_term();
	while (true)
	{
		input = readline("mini> ");
		input_cpy = cstr_to_str(input);
		add_history(input);
		free(input);
		cmds = parse(input_cpy, t);
		exec(cmds, t);
		t->cmds = cmds;
		reset_term(&t);
		freen((void *)&input_cpy.s);
		//break ; //temporário para verificar leaks sem ter de apanhar sinais
	}
	free(t);
}

static t_terminal *init_term(void)
{
	t_terminal *t;

	//AFAZER: libertar
	t = malloc(sizeof(t_terminal));
	t->cmds = NULL;
	t->cmds_num = 1;
	t->terminal_fd = dup(STDOUT);
	return (t);
}

static void reset_term(t_terminal **t)
{
	size_t i;

	if (!t || !*t)
		return ;
	//libertar comandos
	if ((*t)->cmds)
	{
		i = 0;
		while (i < (*t)->cmds_num)
		{
			free((*t)->cmds[i].args);
			q_destroy(&(*t)->cmds[i].redirs, NULL);
			i++;
		}
		free((*t)->cmds);
		(*t)->cmds = NULL;
	}
}
