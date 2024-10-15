#include "minishell.h"

static t_terminal *init_term(void);

int g_sig_received = 0;

int main(void)
{ 

	char *input;
	t_terminal *t;

	t = init_term();
	load_signals();
	while (true)
	{
		printf("exit code: %d\n", t->exit_code);
		input = readline("minishell> ");
		t->input_cpy = cstr_to_str(input);
		if (!t->input_cpy.s)
		{
			freen((void *)&t->input_cpy.s);
			reset_term(&t);
			break ;
		}
		if (g_sig_received)
		{
			freen((void *)&t->input_cpy.s);
			reset_term(&t);
			t->exit_code = 130;
			g_sig_received = 0;
			continue ;
		}
		add_history(input);
		free(input);
		t->cmds = parse(t->input_cpy, t);
		if (t->cmds)
			t->exit_code = exec(t->cmds, t);
		else
		{};/*não faz nada*///ft_fprintf(ERROR, "Format error\n");
		reset_term(&t);
		freen((void *)&t->input_cpy.s);
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
	t->exit_code = 0;
	rl_catch_signals = 1;
	return (t);
}

void reset_term(t_terminal **t)
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
			ft_lstclear(&(*t)->cmds[i].redirs, free);
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

void debug_args(t_cmd *cmds, size_t cmds_num)
{
	for (size_t i = 0; i < cmds_num; ++i)
	{
		for (size_t j = 0; cmds[i].args[j].s; ++j)
			ft_fprintf(STDOUT, "%S\n", cmds[i].args[j]);
		ft_fprintf(STDOUT, "\n");
	}
}
