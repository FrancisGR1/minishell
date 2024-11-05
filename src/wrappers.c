#include "minishell.h"

void ft_add_history(t_string input)
{
	if (g_sig_received)
		return ;
	if ((int) input.len == str_iter(input, 0, input.len, ft_isspace))
		return ;
	add_history(input.s);
}

t_string	ft_readline(char *prompt)
{
	char		*tmp;
	t_string	input;

	tmp = readline(prompt);
	input = cstr_to_str(tmp);
	freen((void *)&tmp);
	return (input);
}
