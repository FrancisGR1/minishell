/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:47 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:16 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char *command)
{
	static const char	*builtins[] = {"echo", "unset", "env", "export", "pwd",
		"exit", "cd", NULL};
	size_t				i;

	i = 0;
	while (builtins[i])
	{
		if (ft_strcmp(command, builtins[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}

int	exec_builtin(char **argv, int argc, t_terminal *t)
{
	int	exit;

	if (!argv || !argv[0] || !t)
		return (0);
	exit = 0;
	if (!ft_strcmp(argv[0], "echo"))
		exit = builtin_echo(argv, argc);
	else if (!ft_strcmp(argv[0], "unset"))
		exit = builtin_unset(argv, argc, t);
	else if (!ft_strcmp(argv[0], "env"))
		exit = builtin_env(t);
	else if (!ft_strcmp(argv[0], "export"))
		exit = builtin_export(argv, argc, t);
	else if (!ft_strcmp(argv[0], "pwd"))
		exit = builtin_pwd();
	else if (!ft_strcmp(argv[0], "exit"))
		exit = builtin_exit(argv, argc, t);
	else if (!ft_strcmp(argv[0], "cd"))
		exit = builtin_cd(argv, argc, t);
	if (!should_exec_in_main(argv, t))
		freexit(exit, t);
	return (exit);
}

bool	should_exec_in_main(char **argv, t_terminal *t)
{
	bool	heredoc_no_error;

	if (!argv || !t)
		return (false);
	heredoc_no_error = t->cmds && t->cmds[0].ri.heredoc_wstatus < FATAL_ERROR;
	return (t->cmds_num == 1 && heredoc_no_error && (!ft_strcmp(argv[0], "cd")
			|| !ft_strcmp(argv[0], "exit") || (!ft_strcmp(argv[0], "export")
				&& t->cmds->cstr_argc > 1) || !ft_strcmp(argv[0], "unset")));
}

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
