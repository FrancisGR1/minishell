/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:18 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*parse(t_string input, t_terminal *t)
{
	t_parser_buffer	pb;

	init_parser(&pb, t);
	if (!!mark_special_characters(input, &t->cmds_num))
		return (frerror(WRONG_FORMAT, ERROR_QUOTES, &pb));
	pb.cmds = ft_calloc((t->cmds_num + 1), sizeof(t_cmd));
	pb.pipe_sides = string_split(input, "\1", &pb.split_len);
	if (!pb.pipe_sides)
		return (frerror(NO_INPUT, NULL, &pb));
	while (++pb.idx < (int)t->cmds_num)
	{
		init_cmds(&pb, pb.idx);
		pb.args_ptr = get_args_ptrs(&pb);
		if (!pb.args_ptr)
			return (frerror(WRONG_FORMAT, ERROR_NO_CMD, &pb));
		while (pb.redir_ptrs && ++pb.redir_idx < (int)pb.redir_ptrs->len)
			if (!format_args(&pb, &pb.cmds[pb.idx], &pb.redir_idx))
				return (frerror(WRONG_FORMAT, ERROR_NO_REDIR, &pb));
		if (!set_cmd(pb.cmds, pb.idx, pb.args_ptr, t) && !pb.redir_ptrs)
			return (frerror(WRONG_FORMAT, ERROR_NO_CMD, &pb));
		darr_free(pb.redir_ptrs);
	}
	alloc_args(pb.cmds, t->cmds_num, t->env);
	free(pb.pipe_sides);
	return (pb.cmds);
}
