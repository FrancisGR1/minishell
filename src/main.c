/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:16 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig_received = 0;

int	main(int c, char **v, char **env)
{
	t_terminal	*t;

	(void)v;
	if (c > 1)
		return (EXIT_FAILURE);
	t = init_term(env);
	while (true)
	{
		load_signals(DEFAULT);
		t->input = ft_readline(t->prompt);
		if (!t->input.s)
			break ;
		ft_add_history(t->input);
		t->cmds = parse(t->input, t);
		if (t->cmds && t->cmds_num < CMD_MAX)
			t->exit_code = exec(t->cmds, t);
		else if (g_sig_received)
			t->exit_code = g_sig_received + FATAL_ERROR;
		// TODO: exit() aqui?
		reset_term(&t);
	}
	return (destroy_term(&t));
}
