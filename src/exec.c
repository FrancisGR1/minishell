/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:34 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 21:11:11 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_exec_error(int redir_error, int idx, t_cmd *cmds,
				t_terminal *t);
static void	exec_subprocess(int fds[][2], t_cmd *cmds, int idx, t_terminal *t);
static int	end_execution(pid_t pids[], t_cmd *cmds, int curr_cmd,
				t_terminal *t);

int	exec(t_cmd *cmds, t_terminal *t)
{
	int		fds[CMD_MAX][2];
	pid_t	pids[CMD_MAX];
	int		i;

	init_pipes(fds, t->cmds_num - 1);
	i = -1;
	while (++i < (int)t->cmds_num)
	{
		load_subprocess_signals(&cmds[i], t);
		pids[i] = fork();
		if (pids[i] == SUBPROCESS)
		{
			if (cmds[i].ri.has_heredoc)
				load_signals(HEREDOC);
			exec_subprocess(fds, cmds, i, t);
		}
		if (cmds[i].ri.has_heredoc)
			wait_heredoc(&cmds[i].ri.heredoc_wstatus, pids[i]);
		if (cmds[i].ri.heredoc_wstatus >= FATAL_ERROR)
			break ;
		if (should_exec_in_main(cmds[i].cstr_args, t))
			t->exit_code = exec_builtin(cmds[i].cstr_args, cmds[i].cstr_argc,
					t);
	}
	return (close_fds(fds, t->cmds_num - 1), end_execution(pids, cmds, i, t));
}

bool	should_exec(t_cmd *cmds, t_terminal *t)
{
	return ((cmds && t->cmds_num < CMD_MAX) && (t->cmds_num > 1
			|| ((cmds->cstr_args && cmds->cstr_args[0]) || cmds->redirs)));
}

static int	end_execution(pid_t pids[], t_cmd *cmds, int curr_cmd,
		t_terminal *t)
{
	if (!cmds || !t)
		return (1);
	if (t->exit_code == GENERAL_ERROR)
	{
		wait_subprocesses(pids, t->cmds_num, cmds);
		return (GENERAL_ERROR);
	}
	if (cmds[curr_cmd].ri.heredoc_wstatus >= FATAL_ERROR)
	{
		wait_subprocesses(pids, t->cmds_num, cmds);
		return (cmds[curr_cmd].ri.heredoc_wstatus);
	}
	return (wait_subprocesses(pids, t->cmds_num, cmds));
}

static void	handle_exec_error(int redir_error, int idx, t_cmd *cmds,
		t_terminal *t)
{
	if (g_sig_received)
		freexit(FATAL_ERROR + g_sig_received, t);
	else if (redir_error)
		freexit(redir_error, t);
	else if (!cmds[idx].cstr_args && cmds[idx].redirs)
		freexit(EXIT_SUCCESS, t);
	else if (!cmds[idx].cstr_args[0] && !cmds[idx].redirs)
		ft_fprintf(ERROR, "Command \'\' not found\n");
	else if (errno == ENOENT)
		ft_fprintf(ERROR, "%s: Command not found\n", cmds[idx].cstr_args[0]);
	else
		perror(cmds[idx].cstr_args[0]);
	if (errno == EACCES)
		freexit(NOT_EXECUTABLE, t);
	else
		freexit(CMD_NOT_FOUND, t);
}

static void	exec_subprocess(int fds[][2], t_cmd *cmds, int idx, t_terminal *t)
{
	int	redir_error;

	if (t->cmds_num != 1)
		dup2_pipe(fds, idx, t->cmds_num - 1);
	redir_error = set_redirs(cmds[idx].redirs, t->terminal_fd, cmds[idx].ri);
	close_fds(fds, t->cmds_num - 1);
	if (g_sig_received || redir_error || !cmds[idx].cstr_args)
		handle_exec_error(redir_error, idx, cmds, t);
	if (!cmds[idx].cstr_args[0])
		handle_exec_error(redir_error, idx, cmds, t);
	if (should_exec_in_main(cmds[idx].cstr_args, t) && !redir_error)
		freexit(EXIT_SUCCESS, t);
	if (is_builtin(cmds[idx].cstr_args[0]))
		exec_builtin(cmds[idx].cstr_args, cmds[idx].cstr_argc, t);
	else
	{
		if (execve(cmds[idx].cstr_args[0], cmds[idx].cstr_args, t->env) == -1)
			handle_exec_error(redir_error, idx, cmds, t);
	}
}
