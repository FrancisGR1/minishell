/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:35 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_sig_received = 0;

t_string	ft_readline(char *prompt);

int	main(int c, char **v, char **env)
{
	(void) v;
	t_terminal	*t;
	int	main_exit_code;

	if (c > 1)
		return (EXIT_FAILURE);
	main_exit_code = 0;
	t = init_term(env);
	load_signals(DEFAULT);
	while (true)
	{
		t->input = ft_readline(RL_PROMPT);
		if (!t->input.s)
			break ;
		add_history(t->input.s);
		t->cmds = parse(t->input, t);
		if (t->cmds && t->cmds_num < CMD_MAX)
			t->exit_code = exec(t->cmds, t);
		else if (g_sig_received)
			t->exit_code = g_sig_received + FATAL_ERROR;
		main_exit_code = t->exit_code;
		reset_term(&t);
	}
	destroy_term(&t);
	return (main_exit_code);
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
