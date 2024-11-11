/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:36 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:17 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_terminal	*init_term(char *program, char **env)
{
	t_terminal	*t;

	t = malloc(sizeof(t_terminal));
	t->cmds = NULL;
	t->all_args_ptrs = NULL;
	t->cmds_num = 1;
	t->terminal_fd = dup(STDOUT);
	t->exit_code = 0;
	t->env_size = 0;
	t->env = env_dup(env, t);
	t->prompt = BGRN "minishell> " RESET;
	t->is_running = true;
	stat(program, &t->stat);
	rl_catch_signals = 1;
	return (t);
}

void	reset_term(t_terminal **t)
{
	size_t	i;

	g_sig_received = 0;
	if (!t || !*t)
		return ;
	if ((*t)->cmds)
	{
		i = 0;
		while (i < (*t)->cmds_num)
		{
			free_cmd_args(&(*t)->cmds[i]);
			ft_lstclear(&(*t)->cmds[i].redirs, free);
			i++;
		}
		freen((void *)&(*t)->cmds);
	}
	ft_lstclear(&(*t)->all_args_ptrs, NULL);
	string_free(&(*t)->input);
	(*t)->cmds_num = 1;
}

// frees main struct and returns the last exit code
int	destroy_term(t_terminal **t)
{
	int		main_exit_code;
	char	**env;

	if (!t)
		return (0);
	main_exit_code = (*t)->exit_code;
	env = (*t)->env;
	while (env && *env)
		free(*env++);
	free((*t)->env);
	safe_close((*t)->terminal_fd);
	freen((void *)&(*t));
	return (main_exit_code);
}

bool term_should_stop_running(t_terminal *t)
{
	if (!t->cmds || !t->cmds->cstr_args)
		return (false);
	if (t->cmds_num == 1 && !ft_strcmp(t->cmds[0].cstr_args[0], "exit")
			&& t->exit_code != DONT_EXIT && !g_sig_received)
	{
		ft_fprintf(STDOUT, "exit\n");
		return (true);
	}
	if (t->exit_code == DONT_EXIT)
		t->exit_code = GENERAL_ERROR;
	return (false);
}

bool is_nested_term(t_cmd cmd, t_terminal *t)
{
	struct stat prog_info;

	if (!cmd.cstr_args)
		return (false);
	if (stat(cmd.cstr_args[0], &prog_info) == -1)
		return (false);
	return (prog_info.st_ino == t->stat.st_ino &&
			prog_info.st_dev == t->stat.st_dev);
}
