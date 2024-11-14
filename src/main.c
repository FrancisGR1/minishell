/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:18 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	g_sig_received = 0;

int	main(int c, char **v, char **env)
{
	t_terminal	*t;

	if (c > 1)
		return (EXIT_FAILURE);
	t = init_term(v[0], env);
	while (t->is_running)
	{
		load_signals(DEFAULT);
		t->input = ft_readline(t->prompt, t);
		if (!t->input.s)
			break ;
		t->cmds = parse(t->input, t);
		if (should_exec(t->cmds, t))
			t->exit_code = exec(t->cmds, t);
		if (term_should_stop_running(t))
			t->is_running = false;
		reset_term(&t);
	}
	return (destroy_term(&t));
}
