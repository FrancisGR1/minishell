#include "minishell.h"

int g_sig_received = 0;

t_string ft_readline(char *prompt, t_terminal *t);

int main(void)
{ 
	t_terminal *t;

	t = init_term();
	load_signals();
	while (true)
	{
		t->input = ft_readline(RL_PROMPT, t);
		if (!t->input.s)
			break ;
		add_history(t->input.s);
		t->cmds = parse(t->input, t);
		//printf("debugging args:\n");
		//debug_args(t->cmds, t->cmds_num);
		//printf("debugging redirs:\n");
		//debug_redirections(t->.cmds, t->cmds_num);

		if (t->cmds)
			t->exit_code = exec(t->cmds, t);
		reset_term(&t);
	}
	rl_clear_history();
	destroy_term(&t);
}


t_string ft_readline(char *prompt, t_terminal *t)
{
	char *tmp;
	t_string input;

	while (true)
	{
		tmp = readline(prompt);
		input = cstr_to_str(tmp);
		freen((void *)&tmp);
		if (g_sig_received)
		{
			t->exit_code = FATAL_ERROR;
			g_sig_received = 0;
			string_free(&input);
			continue;
		}
		break ;
	}
	return (input);
}
