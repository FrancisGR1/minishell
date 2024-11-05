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
	t_terminal	*t;
	(void) v;

	if (c > 1)
		return (EXIT_FAILURE);
	t = init_term(env);
	load_signals(DEFAULT);
	while (true)
	{
		t->input = ft_readline(RL_PROMPT);
		if (!t->input.s)
			break ;
		//TODO: meter isto num wrapper
		//só incluir história de input for válido
		add_history(t->input.s);
		t->cmds = parse(t->input, t);
		debug_cmds("STR ARGS", t->cmds, t->cmds_num);
		debug_cstr_args("CSTR", t->cmds, t->cmds_num);
		if (t->cmds && t->cmds_num < CMD_MAX)
			t->exit_code = exec(t->cmds, t);
		else if (g_sig_received)
			t->exit_code = g_sig_received + FATAL_ERROR;
		//TODO: exit() aqui?
		reset_term(&t);
	}
	return (destroy_term(&t));
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
