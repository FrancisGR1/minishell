/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:34 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:34 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freexit(int exit_code, t_cmd *cmds, t_terminal *t)
{
	(void) cmds; /*TODO: eliminar*/
	close(t->terminal_fd_input);
	close(t->terminal_fd_output);
	string_free(&t->input);
	reset_term(&t);
	free(t);
	exit(exit_code);
}

void	*free_on_error(int exit_code, char *error_message, t_parser_buffer *pb)
{
	int	i;

	i = pb->idx;
	if (pb->pipe_sides)
		freen((void *)&pb->pipe_sides);
	if (pb->args_ptr)
		freen((void *)&pb->args_ptr);
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

void	free_cmd_cstr_args(t_cmd *cmds)
{
	int	i;

	i = 0;
	if (!cmds || !cmds->cstr_args)
		return ;
	while (cmds->cstr_args[i])
	{
		freen((void *)&cmds->cstr_args[i]);
		i++;
	}
	freen((void *)&cmds->cstr_args);
}

void	alloc_args(t_cmd *cmds, int commands)
{
	int		i;
	int		j;
	int		k;
	char	**cmd_args;

	if (!cmds || !cmds->args || !cmds->args->s)
		return ;
	i = 0;
	while (i < commands)
	{
		j = 0;
		while (cmds[i].args[j].s)
			j++;
		cmd_args = malloc(sizeof(char *) * (j + 1));
		k = 0;
		while (k < j)
		{
			cmd_args[k] = string_convert_back(cmds[i].args[k]);
			k++;
		}
		cmd_args[k] = NULL;
		cmds[i].cstr_args = cmd_args;
		i++;
	}
}
