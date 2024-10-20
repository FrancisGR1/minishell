/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:35 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	format_args(t_parser_buffer *pb, t_cmd *cmds, int *redir_idx);
static bool	set_cmd(t_cmd *cmds, size_t idx, t_string *args_ptr);
static int	mark_special_characters(t_string input, size_t *cmds_num);
static int	remove_quotes(t_string arg);

t_cmd	*parse(t_string input, t_terminal *t)
{
	t_parser_buffer	pb;

	init_parser(&pb, t);
	if (!!mark_special_characters(input, &t->cmds_num))
		return (free_on_error(WRONG_FORMAT, "Format error: Quotes unclosed", &pb));
	pb.cmds = malloc((t->cmds_num + 1) * sizeof(t_cmd));
	pb.pipe_sides = string_split(input, "\1");
	if (!pb.pipe_sides)
		return (free_on_error(NO_INPUT, NULL, &pb));
	while (++pb.idx < (int)t->cmds_num)
	{
		pb.args_ptr = string_split(pb.pipe_sides[pb.idx], DELIMITERS);
		if (!pb.args_ptr)
			return (free_on_error(WRONG_FORMAT, "Format error: Missing command", &pb));
		init_redirs(&pb, pb.idx);
		while (pb.redir_ptrs && ++pb.redir_idx < (int)pb.redir_ptrs->len)
			if (!format_args(&pb, pb.cmds, &pb.redir_idx))
				return (free_on_error(WRONG_FORMAT, "Format error: No redirection file", &pb));
		if (!set_cmd(pb.cmds, pb.idx, pb.args_ptr))
			return (free_on_error(WRONG_FORMAT, "Format error: No command", &pb));
		darr_free(pb.redir_ptrs);
	}
	pb.cmds[pb.idx].binary = new_str(NULL);
	free(pb.pipe_sides);
	return (pb.cmds);
}

static int	remove_quotes(t_string arg)
{
	size_t	j;
	size_t	k;
	char	quote;

	j = 0;
	k = 0;
	quote = '\0';
	while (j < arg.len)
	{
		if (!quote && (arg.s[j] == '\'' || arg.s[j] == '\"'))
			quote = arg.s[j];
		else if (quote && arg.s[j] == quote)
			quote = '\0';
		else
		{
			arg.s[k] = arg.s[j];
			k++;
		}
		j++;
	}
	arg.len -= j - k;
	arg.end -= j - k;
	return (0);
}

int	mark_special_characters(t_string input, size_t *cmds_num)
{
	char	quote;
	size_t	i;

	quote = '\0';
	i = 0;
	while (i < input.len)
	{
		if (!quote && (input.s[i] == '\'' || input.s[i] == '\"'))
			quote = input.s[i];
		else if (input.s[i] == quote)
			quote = '\0';
		if (!quote && input.s[i] == '<')
			input.s[i] = LESS;
		else if (!quote && input.s[i] == '>')
			input.s[i] = MORE;
		else if (!quote && input.s[i] == '|')
			input.s[i] = PIPE;
		else if (!quote && ft_isspace(input.s[i]))
			input.s[i] = SPACE;
		if (!quote && input.s[i] == PIPE)
			(*cmds_num)++;
		i++;
	}
	return (quote);
}

static bool	set_cmd(t_cmd *cmds, size_t idx, t_string *args_ptr)
{
	size_t	i;

	if (cmds[idx].has_heredoc)
	{
		write_path(cmds[idx].heredoc_file, rand_string());
	}
	cmds[idx].binary = args_ptr[0];
	cmds[idx].args = args_ptr;
	if (!cmds[idx].binary.s)
		return (false);
	i = 0;
	while (args_ptr[i].s)
	{
		remove_quotes(args_ptr[i]);
		// TODO: expandir aqui
		// expand(args_ptr[i]);
		i++;
	}
	return (true);
}

static bool	format_args(t_parser_buffer *pb, t_cmd *cmds, int *redir_idx)
{
	if (!get_redir(pb, cmds, redir_idx))
		return (false);
	if (cmds)
		remove_redirections(pb, cmds);
	//TODO: expandir aqui:
	return (true);
}
