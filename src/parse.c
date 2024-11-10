/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:16 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	format_args(t_parser_buffer *pb, t_cmd *current_cmd,
				int *redir_idx);
static bool	set_cmd(t_cmd *cmds, size_t idx, t_string *args_ptr, t_terminal *t);
static int	mark_special_characters(t_string input, size_t *cmds_num);
static int	remove_quotes(t_string *arg);

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
		init_redirs(&pb, pb.idx);
		pb.args_ptr = string_split(pb.pipe_sides[pb.idx], DELIMITERS,
				&pb.split_len);
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
	return (free(pb.pipe_sides), pb.cmds);
}

static int	remove_quotes(t_string *arg)
{
	size_t	j;
	size_t	k;
	char	quote;

	j = 0;
	k = 0;
	quote = '\0';
	if (!arg)
		return (-1);
	while (j < arg->len)
	{
		if (!quote && (arg->s[j] == '\'' || arg->s[j] == '\"'))
			quote = arg->s[j];
		else if (quote && arg->s[j] == quote)
			quote = '\0';
		else
		{
			arg->s[k] = arg->s[j];
			k++;
		}
		j++;
	}
	arg->len -= j - k;
	arg->end -= j - k;
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

static bool	set_cmd(t_cmd *cmds, size_t idx, t_string *args_ptr, t_terminal *t)
{
	size_t	i;
	size_t	argc;

	if (!cmds || !args_ptr || !t)
		return (false);
	i = -1;
	argc = strs_count(args_ptr);
	while (++i < argc)
	{
		if (string_find(args_ptr[i], 0, args_ptr[i].len, "$") >= 0)
		{
			expand(&args_ptr[i], t->env, t->exit_code, 0);
			if (ft_strncmp(args_ptr[0].s, "export", args_ptr[0].len) != 0)
				rearrange_args_after_expansion(&args_ptr, i, &argc);
		}
		remove_quotes(&args_ptr[i]);
		if (args_ptr[i].len <= 0)
		{
			string_free(&args_ptr[i]);
			args_ptr[i] = cstr_to_str(EMPTY_STR);
		}
	}
	cmds[idx].args = args_ptr;
	cmds[idx].argc = argc;
	return (true);
}

static bool	format_args(t_parser_buffer *pb, t_cmd *current_cmd, int *redir_idx)
{
	if (!get_redir(pb, current_cmd, redir_idx))
		return (false);
	if (current_cmd)
		remove_redirections(pb, current_cmd);
	return (true);
}
