#include "minishell.h"

char **env_dup(char **env)
{
	char **env_dup;
	size_t size;
	size_t i;

	if (!env)
		return (NULL);
	size = 200;
	env_dup = malloc(size * sizeof(char *) + 1);
	if (!env_dup)
		return (NULL);
	i = 0;
	while (env[i])
	{
		//TODO: substituir por env_add()
		if (i > size)
		{
			size *= 2;
			env_dup = (char **)ft_realloc(env_dup, size);
		}
		env_dup[i] = ft_strndup(env[i], ft_strlen(env[i]));
		i++;
	}
	env_dup[i] = NULL;
	return (env_dup);
}

char *env_lookup(char **env, char *target)
{
	size_t i;
	const size_t last_idx = ft_strlen(target) - 1;

	if (!env || !target)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], target, last_idx) == 0)
			return (env[i]);
		i++;
	}
	return (NULL);
}
