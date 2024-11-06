/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:48 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:15 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	cleanup_arg(char *str)
{
	size_t	i;
	size_t	j;

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\5' || str[i] == '\6')
		{
			j++;
			str[i] = str[j];
			continue ;
		}
		else
		{
			str[i] = str[j];
			j++;
		}
		i++;
	}
	str[i] = '\0';
}

void	alloc_args(t_cmd *cmds, int commands_num, char **t_env)
{
	int		i;
	int		k;
	int		j;
	char	**cmd_args;

	if (!cmds || !cmds->args)
		return ;
	i = -1;
	while (++i < commands_num)
	{
		cmd_args = ft_calloc(sizeof(char *), (cmds[i].argc + 1));
		k = -1;
		j = 0;
		while (++k < (int)cmds[i].argc)
		{
			if (ft_strcmp(cmds[i].args[k].s, EMPTY_EXPANDED_STR) == 0)
				continue ;
			cmd_args[j] = string_convert_back(cmds[i].args[k]);
			cleanup_arg(cmd_args[j++]);
		}
		cmds[i].cstr_argc = j;
		cmd_args[j] = NULL;
		get_binary_path(cmd_args, t_env);
		cmds[i].cstr_args = cmd_args;
	}
}

t_string	*make_rearranged_args(t_string *old, t_string *split, int curr,
		int c)
{
	t_string	*new_args;
	const int	len = strs_count(split);

	if (!old || !split)
		return (NULL);
	new_args = malloc((c + 1) * sizeof(t_string));
	if (curr == 0)
	{
		ft_memcpy(new_args, split, sizeof(t_string) * len);
		if (c > 1)
			ft_memcpy(&new_args[len], &old[1], sizeof(t_string) * (c - len));
	}
	else if (curr == c - 1)
	{
		ft_memcpy(new_args, old, sizeof(t_string) * (c - 1));
		ft_memcpy(&new_args[c - 1], split, sizeof(t_string) * len);
	}
	else
	{
		ft_memcpy(new_args, old, sizeof(t_string) * curr);
		ft_memcpy(&new_args[curr], split, sizeof(t_string) * len);
		ft_memcpy(&new_args[curr + len], old + curr, sizeof(t_string) * (c - 1
				- curr));
	}
	return (new_args[c] = new_str(NULL, 0), new_args);
}

void	rearrange_args_after_expansion(t_string **arg, int current,
		size_t *argc)
{
	t_string	*split_args;
	t_string	*new_args;
	t_string	*expanded_str_ptr;
	int			arr_len;

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
