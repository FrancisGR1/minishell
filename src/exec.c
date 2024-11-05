/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:34 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:34 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	close_fds(int fds[][2], int cmds_num);
static void	dup2_pipe(int fds[][2], int idx, int last);
static int	wait_subprocesses(pid_t *subprocesses, int commands, t_cmd *cmds);
static void	exec_subprocess(int fds[][2], t_cmd *cmds, int idx, t_terminal *t);

int	exec(t_cmd *cmds, t_terminal *t)
{
	int	command_c = t->cmds_num - 1; //TODO: tirar isto daqui
	int			fds[CMD_MAX][2];
	pid_t		pids[CMD_MAX];
	int			i;

	init_pipes(fds, command_c);
	i = 0;
	while (i < (int)t->cmds_num)
	{
		pids[i] = fork();
		if (pids[i] == SUBPROCESS) 
		{
			exec_subprocess(fds, cmds, i, t);
		}
		if (cmds[i].has_heredoc)
		{
			load_signals(DO_NOTHING);
			wait_heredoc(&cmds[i].heredoc_wstatus, pids[i]);
			load_signals(DEFAULT);
		}
		i++;
	}
	close_fds(fds, command_c);
	return (wait_subprocesses(pids, t->cmds_num, cmds));
}

static void handle_exec_error(int redir_error, int idx, t_cmd *cmds, t_terminal *t)
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
		//TODO: tenho de incluir outros tipos de saídas
		//falta a utilização errada de builtins (código de saída: 2)

}

static void	exec_subprocess(int fds[][2], t_cmd *cmds, int idx, t_terminal *t)
{
	int redir_error;

	if (t->cmds_num != 1)
		dup2_pipe(fds, idx, t->cmds_num - 1);
	redir_error = set_redirs(cmds[idx].redirs, cmds[idx].heredoc_file, t->terminal_fd, cmds[idx].last_input_ptr, cmds[idx].last_output_ptr);
	close_fds(fds, t->cmds_num - 1);
	if (g_sig_received || redir_error || !cmds[idx].cstr_args )
	{
		handle_exec_error(redir_error, idx, cmds, t);
	}
	if (!cmds[idx].cstr_args[0])
	{
		handle_exec_error(redir_error, idx, cmds, t);
	}
	if (execve(cmds[idx].cstr_args[0], cmds[idx].cstr_args, t->env) == -1)
	{
		handle_exec_error(redir_error, idx, cmds, t);
	}
}

static void	dup2_pipe(int fds[][2], int idx, int last)
{
	if (idx == 0)
	{
		dup2(fds[idx][PIPE_WRITE], STDOUT);
	}
	else if (idx == last)
	{
		dup2(fds[idx - 1][PIPE_READ], STDIN);
	}
	else
	{
		dup2(fds[idx - 1][PIPE_READ], STDIN);
		dup2(fds[idx][PIPE_WRITE], STDOUT);
	}
}

static void	close_fds(int fds[][2], int cmds_num)
{
	int	i;

	i = 0;
	while (i < cmds_num)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

static int	wait_subprocesses(pid_t *subprocesses, int commands, t_cmd *cmds)
{
	int	exit_code;
	int	wstatus;
	int	i;

	exit_code = 0;
	wstatus = 0;
	i = 0;
	while (i < commands)
	{
		if (cmds[i].has_heredoc)
		{
			unlink(cmds[i].heredoc_file);
			exit_code = cmds[i].heredoc_wstatus;
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
