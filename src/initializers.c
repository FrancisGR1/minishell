/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initializers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:15 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_parser(t_parser_buffer *pb, t_terminal *t)
{
	pb->pipe_sides = NULL;
	pb->args_ptr = NULL;
	pb->split_len = 0;
	pb->redir_ptrs = NULL;
	pb->idx = -1;
	pb->redir_idx = -1;
	pb->cmds = NULL;
	pb->t = t;
}

void	init_cmds(t_parser_buffer *pb, size_t idx)
{
	pb->cmds[idx].cstr_args = NULL;
	pb->cmds[idx].args = NULL;
	pb->cmds[idx].argc = 0;
	pb->cmds[idx].redirs = NULL;
	pb->cmds[idx].ri.li_ptr = NULL;
	pb->cmds[idx].ri.lo_ptr = NULL;
	pb->cmds[idx].ri.has_heredoc = false;
	pb->cmds[idx].ri.heredoc_wstatus = 0;
	pb->redir_idx = -1;
	pb->redir_ptrs = string_findall(pb->pipe_sides[idx], "\2\3");
	pb->redir_ptr = NULL;
}

void	init_pipes(int fds[][2], int cmds_num)
{
	int	i;

	i = 0;
	while (i < cmds_num)
	{
		pipe(fds[i]);
		i++;
	}
}
