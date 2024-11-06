/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_wait.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:47 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:15 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_subprocesses(pid_t *subprocesses, int commands, t_cmd *cmds)
{
	int	exit_code;
	int	wstatus;
	int	i;

	exit_code = 0;
	wstatus = 0;
	i = 0;
	while (i < commands)
	{
		if (cmds[i].ri.has_heredoc)
		{
			unlink(cmds[i].ri.heredoc_file);
			exit_code = cmds[i].ri.heredoc_wstatus;
		}
		else
		{
			waitpid(subprocesses[i], &wstatus, 0);
			if (WIFSIGNALED(wstatus))
				exit_code = FATAL_ERROR + WTERMSIG(wstatus);
			else if (WIFEXITED(wstatus))
				exit_code = WEXITSTATUS(wstatus);
		}
		i++;
	}
	return (exit_code);
}

void	wait_heredoc(int *hd_exit_status, pid_t pid)
{
	int	wstatus;

	waitpid(pid, &wstatus, 0);
	if (WIFSIGNALED(wstatus))
	{
		*hd_exit_status = FATAL_ERROR + WTERMSIG(wstatus);
	}
	else if (WIFEXITED(wstatus))
	{
		*hd_exit_status = WEXITSTATUS(wstatus);
	}
}
