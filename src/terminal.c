#include "minishell.h"

t_terminal *init_term(void)
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
			free_cmd_args((*t)->cmds, (int) (*t)->cmds_num);
			ft_lstclear(&(*t)->cmds[i].redirs, free);
			i++;
		}
		free((*t)->cmds);
		(*t)->cmds = NULL;
	}
	freen((void *)&(*t)->input.s);
	(*t)->cmds_num = 1;
}

void destroy_term(t_terminal **t)
{
	close((*t)->terminal_fd);
	freen((void *)&(*t));
}
