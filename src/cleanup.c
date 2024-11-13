/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:34 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:15 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freexit(int exit_code, t_terminal *t)
{
	safe_close(t->terminal_fd);
	string_free(&t->input);
	reset_term(&t);
	destroy_term(&t);
	free(t);
	exit(exit_code);
}

void	*frerror(int exit_code, char *error_message, t_parser_buffer *pb)
{
	int	i;

	i = pb->idx;
	if (pb->pipe_sides)
		freen((void *)&pb->pipe_sides);
	ft_lstclear(&pb->t->all_args_ptrs, free);
	if (pb->redir_ptrs)
		darr_free(pb->redir_ptrs);
	while (i >= 0)
	{
		ft_lstclear(&pb->cmds[i].redirs, free);
		i--;
	}
	freen((void *)&pb->cmds);
	if (error_message)
		ft_fprintf(ERROR, "%s\n", error_message);
	pb->t->exit_code = exit_code;
	return (NULL);
}

void	free_cmd_args(t_cmd *current_cmd)
{
	size_t	i;

	i = 0;
	if (!current_cmd || !current_cmd->cstr_args)
		return ;
	while (i < current_cmd->argc)
	{
		string_free(&current_cmd->args[i]);
		if (i < current_cmd->cstr_argc)
		{
			freen((void *)&current_cmd->cstr_args[i]);
		}
		i++;
	}
	freen((void *)&current_cmd->args);
	freen((void *)&current_cmd->cstr_args);
}
