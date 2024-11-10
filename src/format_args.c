/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   format_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:48 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/09 14:33:38 by frmiguel         ###   ########.fr       */
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

//TODO: fazer func que copia args
t_string *make_rearranged_args(t_string *old, t_string *split, int cur, int c)
{
    t_string *na;
    t_string *old_duped;
    const int len = strs_count(split);
    
    if (!old || !split)
        return (NULL);
        
    // Allocate new array
    na = malloc((c + 1) * sizeof(t_string));
    if (!na)
        return NULL;
        
    // Create deep copy of old array
    old_duped = str_arr_dup(old);
    if (!old_duped)
    {
        free(na);
        return NULL;
    }
    
    // Free original array
    string_arr_free(&old);
    
    // We need to deep copy the strings when copying to na
    if (cur == 0)
    {
        // Copy split strings with deep copy
        for (int i = 0; i < len; i++)
            na[i] = str_dup(split[i]);
            
        // Copy remaining strings from old_duped
        if (c > 1)
            for (int i = 0; i < (c - len); i++)
                na[len + i] = str_dup(old_duped[1 + i]);
    }
    else if (cur == c - 1)
    {
        // Copy prefix from old_duped
        for (int i = 0; i < (c - 1); i++)
            na[i] = str_dup(old_duped[i]);
            
        // Copy split strings at the end
        for (int i = 0; i < len; i++)
            na[c - 1 + i] = str_dup(split[i]);
    }
    else
    {
        // Copy prefix from old_duped
        for (int i = 0; i < cur; i++)
            na[i] = str_dup(old_duped[i]);
            
        // Copy split strings in the middle
        for (int i = 0; i < len; i++)
            na[cur + i] = str_dup(split[i]);
            
        // Copy suffix from old_duped if needed
        if (c > (cur + len))
        {
            for (int i = 0; i < (c - cur - len); i++)
                na[cur + len + i] = str_dup(old_duped[cur + 1 + i]);
        }
    }
    
    // Free the temporary copy
    string_arr_free(&old_duped);
    
    // Set NULL terminator
    na[c] = new_str(NULL, 0);
    
    return na;
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
	debug_args("expanded_str_ptr", expanded_str_ptr);
	if (!expanded_str_ptr)
		return ;
	split_args = string_split_dup(*expanded_str_ptr, " ", &arr_len);
	if (!split_args || arr_len < 2)
		return (string_arr_free(&split_args));
	*argc += arr_len - 1;
	new_args = make_rearranged_args(*arg, split_args, current, *argc);
	string_arr_free(&split_args);
	*arg = new_args;
}
