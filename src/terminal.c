/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   terminal.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:36 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:36 by frmiguel         ###   ########.fr       */
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
	rl_catch_signals = 1;
	return (t);
}

void	reset_term(t_terminal **t)
{
	size_t	i;

	g_sig_received  = 0;
	if (!t || !*t)
		return ;
	if ((*t)->cmds)
	{
		i = 0;
		while (i < (*t)->cmds_num)
		{
			free_cmd_cstr_args(&(*t)->cmds[i]);
			freen((void *)&(*t)->cmds[i].args);
			ft_lstclear(&(*t)->cmds[i].redirs, free);
			i++;
		}
		freen((void *)&(*t)->cmds);
	}
	string_free(&(*t)->input);
	(*t)->cmds_num = 1;
}

void	destroy_term(t_terminal **t)
{
	char **env;
	env = (*t)->env;
	while (env && *env)
		free(*env++);
	free((*t)->env);
	close((*t)->terminal_fd);
	freen((void *)&(*t));
}
