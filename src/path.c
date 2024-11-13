/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:09:43 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:18 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*find_path(char *cmd, char **env)
{
	size_t		i;
	size_t		split_path_size;
	const char	**split_paths = (const char **)ft_split(env_lookup(env, "PATH",
				VALUE), ":");
	char		path_to_be_written[PATH_MAX];
	char		*res;

	if (!split_paths || !cmd || !cmd[0])
		return (freen_arr((void **)split_paths), NULL);
	res = NULL;
	i = 0;
	while (!res && split_paths[i])
	{
		split_path_size = ft_strlen(split_paths[i]);
		ft_strlcpy(path_to_be_written, split_paths[i], split_path_size + 1);
		ft_strlcat(path_to_be_written, "/", split_path_size + 2);
		ft_strlcat(path_to_be_written, cmd, split_path_size + ft_strlen(cmd)
			+ 2);
		if (access(path_to_be_written, F_OK) == 0)
			res = ft_strdup(path_to_be_written);
		i++;
	}
	freen_arr((void **)split_paths);
	return (res);
}

void	get_binary_path(char **cmd_args, char **t_env)
{
	char	*binary_path;

	if (!cmd_args || !cmd_args[0] || !cmd_args[0][0])
	{
		return ;
	}
	if (is_builtin(cmd_args[0]))
	{
		return ;
	}
	binary_path = find_path(cmd_args[0], t_env);
	if (!binary_path)
		return ;
	free(cmd_args[0]);
	cmd_args[0] = binary_path;
}
