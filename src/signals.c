#include "minishell.h"

int event_hook(void)
{
	return (0);
}

void load_signals(void)
{
	struct sigaction sa;

	sigemptyset(&sa.sa_mask);
	signal(SIGQUIT, SIG_IGN);
	sa.sa_flags = SA_SIGINFO | SA_RESTART;
	sa.sa_sigaction = signals_handler;
	sigaction(SIGINT, &sa, NULL);
	rl_event_hook = event_hook;
}

void signals_handler(int signum, siginfo_t *inf, void *ctx)
{
	(void) inf;
	(void) ctx;
	if (signum == SIGINT)
	{
		g_sig_received = signum;
		rl_done = 1;
	}
}
