#include "minishell.h"

void	dup2_pipe(int fds[][2], int idx, int last)
{
	if (idx == 0)
	{
		safe_dup2(fds[idx][PIPE_WRITE], STDOUT);
	}
	else if (idx == last)
	{
		safe_dup2(fds[idx - 1][PIPE_READ], STDIN);
	}
	else
	{
		safe_dup2(fds[idx - 1][PIPE_READ], STDIN);
		safe_dup2(fds[idx][PIPE_WRITE], STDOUT);
	}
}

void	close_fds(int fds[][2], int cmds_num)
{
	int	i;

	i = 0;
	while (i < cmds_num)
	{
		safe_close(fds[i][0]);
		safe_close(fds[i][1]);
		i++;
	}
}
