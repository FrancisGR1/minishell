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
size_t strs_count(t_string *args);

static bool	format_args(t_parser_buffer *pb, t_cmd *cmds, int *redir_idx);
static bool	set_cmd(t_cmd *cmds, size_t idx, t_string *args_ptr, t_terminal *t);
static int	mark_special_characters(t_string input, size_t *cmds_num);
static int	remove_quotes(t_string *arg);

// Stores the temporary data for/from parsing and
// the important data for convenience

t_cmd	*parse(t_string input, t_terminal *t)
{
	t_parser_buffer	pb;

	init_parser(&pb, t);
	if (!!mark_special_characters(input, &t->cmds_num))
		return (free_on_error(WRONG_FORMAT, "Format error: Quotes unclosed", &pb));
	pb.cmds = ft_calloc((t->cmds_num + 1), sizeof(t_cmd));
	pb.pipe_sides = string_split(input, "\1");
	if (!pb.pipe_sides)
		return (free_on_error(NO_INPUT, NULL, &pb));
	while (++pb.idx < (int)t->cmds_num)
	{
		init_redirs(&pb, pb.idx);
		pb.args_ptr = string_split(pb.pipe_sides[pb.idx], DELIMITERS);
		if (!pb.args_ptr)
			return (free_on_error(WRONG_FORMAT, "Format error: Missing command", &pb));
		while (pb.redir_ptrs && ++pb.redir_idx < (int)pb.redir_ptrs->len)
			if (!format_args(&pb, &pb.cmds[pb.idx], &pb.redir_idx))
				return (free_on_error(WRONG_FORMAT, "Format error: No redirection file", &pb));
		if (!set_cmd(pb.cmds, pb.idx, pb.args_ptr, t) && !pb.redir_ptrs)
			return (free_on_error(WRONG_FORMAT, "Format error: No command", &pb));
		darr_free(pb.redir_ptrs);
	}
	pb.cmds[pb.idx].binary = new_str(NULL, 0);
	free(pb.pipe_sides);
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

	if (!cmds || !args_ptr || !t)
		return (false);
	if (cmds[idx].has_heredoc)
	{
		write_path(cmds[idx].heredoc_file, rand_string());
	}
	cmds[idx].binary = args_ptr[0];
	cmds[idx].args = args_ptr;
	cmds[idx].argc = strs_count(args_ptr); 
	printf("BEFORE:\n");
	i = 0;
	while (i < cmds[idx].argc)
	{
		remove_quotes(&args_ptr[i]);
		//se o string for "", remove_quotes() vai diminuir o tamanho mas 
		//não vai substituir por nulo, o que leva a comportamento indefinido
		//portanto faço isso aqui
		if (args_ptr[i].len == 0)
		{
			string_free(&args_ptr[i]);
			args_ptr[i] = new_str(NULL, 0);
		}
		expand(&args_ptr[i], t->env, t->exit_code, 0);
		//TODO: depois da expansão tenho de voltar a dividir por 
		//espaços e adicionar os argumentos; ver exemplo em todo.md
		//não deve remover o primeiro argumento porque pode ser ''
		if (remove_empty_args(&args_ptr[i], i, &cmds[idx].argc))
			continue ;
		i++;
	}
	printf("AFTER:\n");
	char *res;
	if (args_ptr[0].s)
		res = find_path(args_ptr[0], t->env);
	else
		res = NULL;
	if (res)
	{
		if (args_ptr[0].type == STR_ALLOCATED)
			string_free(&args_ptr[0]);
		args_ptr[0] = cstr_to_str(res);
		freen((void *)&res);
	}
	return (true);
}

static bool	format_args(t_parser_buffer *pb, t_cmd *cmds, int *redir_idx)
{
	if (!get_redir(pb, cmds, redir_idx))
		return (false);
	if (cmds)
		remove_redirections(pb, cmds);
	return (true);
}

size_t strs_count(t_string *args)
{
	size_t i;

	i = 0;
	while (args[i].s)
		i++;
	return(i);
}

//se o argumento for nulo significa que era um string vazio, logo remover
bool remove_empty_args(t_string *arg, int current, size_t *argc)
{
	size_t nbytes;
	t_string ptr;

	if (!arg || arg->len > 0 || current == 0)
		return (false);
	ptr = *arg;
	nbytes = (*argc - current) * sizeof(t_string);
	ft_memmove(arg, arg + 1, nbytes);
	(*argc)--;
	string_free(&ptr);
	return (true);
}
