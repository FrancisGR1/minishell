#include "minishell.h"

char *find_path(char *cmd, char **env)
{
	size_t i;
	size_t split_path_size;
	const char **split_paths = (const char **)ft_split(env_lookup(env, "PATH"), ":");
	char path_to_be_written[PATH_MAX];
	char *res; 

	if (!split_paths || !cmd || !cmd[0])
		return (freen_arr((void **)split_paths), freen((void *)&cmd), NULL);
	res = NULL;
	i = 0;
	while (!res && split_paths[i])
	{
		split_path_size = ft_strlen(split_paths[i]);
		ft_strlcpy(path_to_be_written, split_paths[i], split_path_size + 1);
		ft_strlcat(path_to_be_written, "/", split_path_size + 2);
		ft_strlcat(path_to_be_written, cmd, split_path_size + ft_strlen(cmd) + 2);
		if (access(path_to_be_written, F_OK) == 0)
			res = ft_strdup(path_to_be_written);
		i++;
	}
	freen_arr((void **)split_paths);
	return (res);
}

//TODO: mudar isto de sítio e nome
//(é para o heredoc)
void	write_path(char dest[], char *src)
{
	const char		*current_path = (const char *)getcwd(NULL, 0);
	const size_t	current_path_size = ft_strlen(current_path);
	const size_t	src_size = ft_strlen(src);
	const size_t	total_size = current_path_size + src_size + 1;

	if (!current_path || total_size >= PATH_MAX || ft_strlcpy(dest,
				current_path, current_path_size + 1) == 0)
	{
		dest[0] = '\0';
		free((char *)current_path);
		return ;
	}
	free((char *)current_path);
	ft_strlcat(dest, "/", current_path_size + 2);
	ft_strlcat(dest, src, current_path_size + 2 + src_size);
}
