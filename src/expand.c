/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:47 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:15 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int		is_invalid_dollar(char *s);
int				valid_dollar_char(int c);
static t_string	make_expanded_str(t_string *s, t_string expanded_dollar,
					t_string delimiter);
static t_string	expand_dollar(t_string delimiter, char **env, int exit_code);

// holds most of the temporary variables for expansion
typedef struct s_expand_buffer
{
	int			offset;
	t_string	delimiter;
	t_string	expanded_dollar;
	t_string	united_str;
	t_string	tmp;
	int			next_start;
}				t_expand_buf;

void	expand(t_string *s, char **env, int exit_code, int start)
{
	const int		dollar_pos = string_find(*s, start, s->len, "$");
	const int		dollar_end_pos = str_iter(*s, dollar_pos + 1, s->len - 1
			- dollar_pos, valid_dollar_char);
	t_expand_buf	e;

	if (!s || dollar_pos < 0 || !s->s || *s->s == '\'' || s->len == 0)
		return ;
	e.offset = dollar_end_pos - dollar_pos;
	if (is_invalid_dollar(s->s + dollar_pos))
		return (expand(s, env, exit_code, dollar_pos + 1));
	e.delimiter = cstr_to_str_ptr(s->s + dollar_pos, e.offset);
	e.expanded_dollar = expand_dollar(e.delimiter, env, exit_code);
	e.united_str = make_expanded_str(s, e.expanded_dollar, e.delimiter);
	remove_empty_codes(&e.united_str);
	e.next_start = dollar_pos + e.expanded_dollar.len;
	e.tmp = *s;
	*s = str_dup(e.united_str);
	string_free(&e.tmp);
	string_free(&e.united_str);
	string_free(&e.expanded_dollar);
	expand(s, env, exit_code, e.next_start);
}

static int	is_invalid_dollar(char *s)
{
	char	next_c;

	if (!s || !(s + 1) || !*s)
		return (1);
	next_c = *(s + 1);
	return (!ft_isalpha(next_c) && next_c != '_' && next_c != '?');
}

static t_string	make_expanded_str(t_string *s, t_string expanded_dollar,
		t_string delimiter)
{
	t_string	united_str;
	t_string	old;
	int			num_of_parts;
	t_string	divided_parts[2];

	string_divide_in_two(*s, divided_parts, delimiter, &num_of_parts);
	if (num_of_parts == 0)
		united_str = str_dup(expanded_dollar);
	else if (num_of_parts == 1 && delimiter.s < divided_parts[0].s)
		united_str = str_cat(expanded_dollar, divided_parts[0]);
	else if (num_of_parts == 1)
		united_str = str_cat(divided_parts[0], expanded_dollar);
	else if (num_of_parts == 2)
	{
		united_str = str_dup(divided_parts[0]);
		old = united_str;
		united_str = str_cat(united_str, expanded_dollar);
		string_free(&old);
		old = united_str;
		united_str = str_cat(united_str, divided_parts[1]);
		string_free(&old);
	}
	return (united_str);
}

static t_string	expand_dollar(t_string delimiter, char **env, int exit_code)
{
	char		*dollar_key;
	char		*dollar_value_ptr;
	char		*num;
	t_string	expanded_dollar;

	dollar_key = string_convert_back(delimiter);
	if (ft_strcmp(dollar_key, "$?") == 0)
	{
		num = ft_itoa(exit_code);
		expanded_dollar = cstr_to_str(num);
		free(num);
		free(dollar_key);
		return (expanded_dollar);
	}
	dollar_value_ptr = env_lookup(env, dollar_key + 1);
	if (!dollar_value_ptr)
		expanded_dollar = cstr_to_str_nsize(EMPTY_EXPANDED_STR, 1);
	else
		expanded_dollar = cstr_to_str(dollar_value_ptr);
	free(dollar_key);
	return (expanded_dollar);
}
