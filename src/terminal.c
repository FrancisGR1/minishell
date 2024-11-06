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

t_terminal	*init_term(char **env)
{
	t_terminal	*t;

	t = malloc(sizeof(t_terminal));
	t->cmds = NULL;
	t->cmds_num = 1;
	t->terminal_fd = dup(STDOUT);
	t->exit_code = 0;
	t->env = env_dup(env);
	t->prompt = BGRN "minishell> " RESET;
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
