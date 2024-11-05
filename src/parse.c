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
//TODO: mudar se sítio 
bool remove_empty_args(t_string *arg, int current, size_t *argc);
void rearrange_args_after_expansion(t_string **arg, int current, size_t *argc);
size_t strs_count(t_string *args);

static bool	format_args(t_parser_buffer *pb, t_cmd *current_cmd, int *redir_idx);
static bool	set_cmd(t_cmd *cmds, size_t idx, t_string *args_ptr, t_terminal *t);
static int	mark_special_characters(t_string input, size_t *cmds_num);
static int	remove_quotes(t_string *arg);

t_cmd	*parse(t_string input, t_terminal *t)
{
	t_parser_buffer	pb;

	init_parser(&pb, t);
	if (!!mark_special_characters(input, &t->cmds_num))
		return (free_on_error(WRONG_FORMAT, "Format error: Quotes unclosed", &pb));
	pb.cmds = ft_calloc((t->cmds_num + 1), sizeof(t_cmd));
	pb.pipe_sides = string_split(input, "\1", &pb.split_len);
	if (!pb.pipe_sides)
		return (free_on_error(NO_INPUT, NULL, &pb));
	while (++pb.idx < (int)t->cmds_num)
	{
		init_redirs(&pb, pb.idx);
		pb.args_ptr = string_split(pb.pipe_sides[pb.idx], DELIMITERS, &pb.split_len);
		if (!pb.args_ptr)
			return (free_on_error(WRONG_FORMAT, "Format error: Missing command", &pb));
		while (pb.redir_ptrs && ++pb.redir_idx < (int)pb.redir_ptrs->len)
			if (!format_args(&pb, &pb.cmds[pb.idx], &pb.redir_idx))
				return (free_on_error(WRONG_FORMAT, "Format error: No redirection file", &pb));
		if (!set_cmd(pb.cmds, pb.idx, pb.args_ptr, t) && !pb.redir_ptrs)
			return (free_on_error(WRONG_FORMAT, "Format error: No command", &pb));
		darr_free(pb.redir_ptrs);
	}
	free(pb.pipe_sides);
	alloc_args(pb.cmds, t->cmds_num, t->env);
	return (pb.cmds);
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
	size_t argc;

	if (!cmds || !args_ptr || !t)
		return (false);
	i = 0;
	argc = strs_count(args_ptr); 
	while (i < argc)
	{
		if (string_find(args_ptr[i], 0, args_ptr[i].len, "$") >= 0)
		{
			expand(&args_ptr[i], t->env, t->exit_code, 0);
			rearrange_args_after_expansion(&args_ptr, i, &argc);
		}
		printf("INDEX: %ld\n", i);
		debug_args("ARGS_PTR", args_ptr);
		remove_quotes(&args_ptr[i]);
		if (args_ptr[i].len <= 0)
		{
			string_free(&args_ptr[i]);
			args_ptr[i] = cstr_to_str(EMPTY_STR);
		}
		i++;
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

size_t strs_count(t_string *args)
{
	size_t i;

	if (!args)
		return (0);
	i = 0;
	while (args[i].s)
		i++;
	return(i);
}

t_string *make_rearranged_args(t_string *old_args, t_string *split_args, int current, int argc)
{
	t_string *new_args;
	const int arr_len = strs_count(split_args);

	if (!old_args || !split_args)
		return (NULL);
	new_args = malloc((argc + 1) * sizeof(t_string));
	if (current == 0)
	{
		ft_memcpy(new_args, split_args, sizeof(t_string) * arr_len);
		if (argc > 1)
			ft_memcpy(&new_args[arr_len], &old_args[1], sizeof(t_string) * (argc - arr_len));
	}
	else if (current == argc - 1)
	{
		ft_memcpy(new_args, old_args, sizeof(t_string) * (argc - 1));
		ft_memcpy(&new_args[argc - 1], split_args, sizeof(t_string) * arr_len);
	}
	else
	{
		ft_memcpy(new_args, old_args, sizeof(t_string) * current);
		ft_memcpy(&new_args[current], split_args, sizeof(t_string) * arr_len);
		ft_memcpy(&new_args[current + arr_len], old_args + current, sizeof(t_string) * (argc - 1 - current));
	}
	new_args[argc] = new_str(NULL, 0);
	return (new_args);
}

void rearrange_args_after_expansion(t_string **arg, int current, size_t *argc)
{
	t_string *split_args;
	t_string *new_args;
	t_string *expanded_str_ptr;
	int arr_len;

	if (!arg || !*arg || !(*arg)->s || !argc || *argc <= 0)
		return ;
	if (*(*arg)->s == '\'' || *(*arg)->s == '\"')
		return ;
	expanded_str_ptr = *arg + current;
	if (!expanded_str_ptr)
		return ;
	split_args = string_split_dup(*expanded_str_ptr, " ", &arr_len);
	if (!split_args || arr_len < 2)
		return (string_arr_free(&split_args));
	*argc += arr_len - 1;
	new_args = make_rearranged_args(*arg, split_args, current, *argc);
	string_free((t_string *)expanded_str_ptr);
	string_arr_free(arg);
	free(split_args);
	*arg = new_args;
}

