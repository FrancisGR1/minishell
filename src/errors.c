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
	close(t->terminal_fd);
	string_free(&t->input);
	reset_term(&t);
	destroy_term(&t);
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
	size_t	i;

	i = 0;
	if (!cmds || !cmds->cstr_args)
		return ;
	while (i < cmds->argc)
	{
		freen((void *)&cmds->cstr_args[i]);
		i++;
	}
	freen((void *)&cmds->cstr_args);
}

//TODO: mudar função de sítio
void	alloc_args(t_cmd *cmds, int commands_num)
{
	int		i;
	int		k;
	char	**cmd_args;

	if (!cmds || !cmds->args)
		return ;
	i = -1;
	while (++i < commands_num)
	{
		if (cmds[i].argc == 0)
		{
			cmds[i].cstr_args = NULL;
			continue ;
 		}
		cmd_args = ft_calloc(sizeof(char *),  (cmds[i].argc + 1));
		k = -1;
		while (++k < (int) cmds[i].argc)
		{
			cmd_args[k] = string_convert_back(cmds[i].args[k]);
			if (cmds[i].args[k].type == STR_ALLOCATED)
				string_free(&cmds[i].args[k]);
		}
		cmd_args[k] = NULL;
		cmds[i].cstr_args = cmd_args;
	}
}
