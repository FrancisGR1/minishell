/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 00:30:41 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:30:42 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_string	*get_args_ptrs(t_parser_buffer *pb)
{
	t_string	*args_ptrs;

	args_ptrs = string_split(pb->pipe_sides[pb->idx], DELIMITERS,
			&pb->split_len);
	if (!args_ptrs)
		return (NULL);
	ft_lstadd_back(&pb->t->all_args_ptrs, ft_lstnew(args_ptrs));
	return (args_ptrs);
}

int	remove_quotes(t_string *arg)
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

bool	set_cmd(t_cmd *cmds, size_t idx, t_string *args_ptr, t_terminal *t)
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

bool	format_args(t_parser_buffer *pb, t_cmd *current_cmd, int *redir_idx)
{
	if (!get_redir(pb, current_cmd, redir_idx))
		return (false);
	if (current_cmd)
		remove_redirections(pb, current_cmd);
	return (true);
}
