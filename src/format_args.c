#include "minishell.h"

void cleanup_arg(char *str)
{
	size_t i;
	size_t j;

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
			continue;
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
		cmd_args = ft_calloc(sizeof(char *),  (cmds[i].argc + 1));
		k = -1;
		j = 0;
		while (++k < (int) cmds[i].argc)
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

