#include "minishell.h"

static t_terminal *init_term(void);
static void reset_term(t_terminal **t);

int g_sig_received = 0;

int main(void)
{ 

	t_cmd *cmds;
	char *input;
	t_string input_cpy;
	t_terminal *t;

	t = init_term();
	load_signals();
	while (true)
	{
		input = readline("minishell> ");
		input_cpy = cstr_to_str(input);
		if (!input_cpy.s)
		{
			free(input_cpy.s);
			break ;
		}
		if (g_sig_received)
		{
			free(input_cpy.s);
			g_sig_received = 0;
			continue ;
		}
		add_history(input);
		free(input);
		cmds = parse(input_cpy, t);
		exec(cmds, t);
		t->cmds = cmds;
		reset_term(&t);
		freen((void *)&input_cpy.s);
	}
	rl_clear_history();
	close(t->terminal_fd);
	free(t);
}
static t_terminal *init_term(void)
{
	t_terminal *t;

	t = malloc(sizeof(t_terminal));
	t->cmds = NULL;
	t->cmds_num = 1;
	t->terminal_fd = dup(STDOUT);
	rl_catch_signals = 1;
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
	(*t)->cmds_num = 1;
}

void debug_fds(const char *message) 
{
	char proc_path[64];
	char target[1024];
	printf("\n=== %s ===\n", message);

	for (int fd = 0; fd < 10; fd++) 
	{
		snprintf(proc_path, sizeof(proc_path), "/proc/self/fd/%d", fd);
		ssize_t len = readlink(proc_path, target, sizeof(target) - 1);
		if (len != -1) 
		{
			target[len] = '\0';
			printf("FD %d -> %s\n", fd, target);
		}
	}
	printf("\n");
}


