/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:36 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:36 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	event_hook(void)
{
	return (0);
}

void	load_signals(int at)
{
	static struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	if (at & DO_NOTHING)
	{
		signal(SIGQUIT, SIG_IGN);
	}
	else if (at & BLOCK)
	{
		sa.sa_sigaction = signals_handler;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
	}
	else
	{
		sa.sa_sigaction = signals_handler;
		sigaction(SIGINT, &sa, NULL);
		signal(SIGQUIT, SIG_IGN);
	}
	rl_event_hook = event_hook;
}

void	signals_handler(int signum, siginfo_t *inf, void *ctx)
{
	(void) ctx;
	if (signum == SIGINT)
	{
		if (inf->si_pid == SUBPROCESS)
		{
			ft_fprintf(STDOUT, "\n");
		}
		g_sig_received = signum;
		rl_done = 1;
	}
	if (signum == SIGQUIT)
	{
		ft_fprintf(STDOUT, "Quit (core dumped)\n");
	}
}
