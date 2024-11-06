/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:47 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:14 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**env_dup(char **env)
{
	char	**env_dup;
	size_t	size;
	size_t	i;

	if (!env)
		return (NULL);
	size = 200;
	env_dup = ft_calloc(size, sizeof(char *) + 1);
	if (!env_dup)
		return (NULL);
	i = 0;
	while (env[i])
	{
		// TODO: substituir por env_add()
		if (i > size)
		{
			size *= 2;
			env_dup = (char **)ft_realloc(env_dup, size / 2, size);
		}
		env_dup[i] = ft_strndup(env[i], ft_strlen(env[i]));
		i++;
	}
	env_dup[i] = NULL;
	return (env_dup);
}

char	*env_lookup(char **env, char *key)
{
	size_t	key_len;
	char	*equals_ptr;

	if (!env || !key)
		return (NULL);
	key_len = ft_strlen(key);
	if (!key_len)
		return (NULL);
	// TEMPORÁRIO para testes
	if (!ft_strcmp(key, "a"))
		return ("ls -la");
	if (!ft_strcmp(key, "c"))
		return ("ls");
	while (*env)
	{
		equals_ptr = ft_strchr(*env, '=');
		if (equals_ptr && (size_t)(equals_ptr - *env) == key_len
			&& ft_strncmp(*env, key, key_len) == 0)
			return (++equals_ptr);
		env++;
	}
	return (NULL);
}
