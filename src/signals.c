/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:36 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:19 by frmiguel         ###   ########.fr       */
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
	sa.sa_sigaction = signals_handler;
	sigaction(SIGINT, &sa, NULL);
	if (at & DO_NOTHING)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, SIG_IGN);
	}
	else if (at & HEREDOC)
	{
		sa.sa_sigaction = heredoc_handler;
		signal(SIGQUIT, SIG_IGN);
	}
	else if (at & BLOCK)
	{
		sigaction(SIGQUIT, &sa, NULL);
	}
	else
	{
		signal(SIGQUIT, SIG_IGN);
	}
	rl_event_hook = event_hook;
}

void	heredoc_handler(int signum, siginfo_t *inf, void *ctx)
{
	(void)ctx;
	(void)inf;
	if (signum == SIGINT)
	{
		g_sig_received = signum;
		rl_done = 1;
	}
	if (signum == SIGQUIT)
		return ;
}

void	signals_handler(int signum, siginfo_t *inf, void *ctx)
{
	(void)ctx;
	if (signum == SIGINT)
	{
		if (inf->si_pid == SUBPROCESS)
		{
			ft_fprintf(STDOUT, "\n");
			return ;
		}
		g_sig_received = signum;
		rl_done = 1;
	}
	if (signum == SIGQUIT)
	{
		ft_fprintf(STDOUT, "Quit (core dumped)\n");
	}
}

void	load_subprocess_signals(t_cmd *current_cmd, t_terminal *t)
{
	if (!current_cmd || !t)
		return ;
	if (current_cmd->ri.has_heredoc || is_nested_term(*current_cmd, t))
		load_signals(DO_NOTHING);
	else
		load_signals(BLOCK);
}
