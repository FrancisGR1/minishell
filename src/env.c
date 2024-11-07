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

char	**env_dup(char **env, t_terminal *t)
{
	char	**env_dup;
	size_t	capacity;
	size_t	i;

	if (!env)
		return (NULL);
	capacity = ENV_INITIAL_SIZE;
	env_dup = ft_calloc(capacity, sizeof(char *) + 1);
	if (!env_dup)
		return (NULL);
	i = 0;
	while (env[i])
	{
		if (i == capacity - 1)
		{
			capacity *= 2;
			env_dup = (char **)ft_realloc(env_dup, i, capacity);
		}
		env_dup[i] = ft_strndup(env[i], ft_strlen(env[i]));
		i++;
	}
	t->env_capacity = capacity;
	t->env_size = i;
	env_dup[i] = NULL;
	return (env_dup);
}

char	*env_lookup(char **env, char *key, int should_lookup)
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
		{
			if (should_lookup == VALUE)
				return (++equals_ptr);
			else
				return (*env);
		}
		env++;
	}
	return (NULL);
}

//int setenv(const char *name, const char *value, int overwrite);
void env_set(char **env, char *key, char *new_value, t_terminal *t)
{
	char *key_exists;
	char *new_key_and_value;

	if (!t || !t->env || !key || !new_value)
		return ;
	key_exists = env_lookup(t->env, key, VALUE);
	if (key_exists)
	{
		if (ft_strlen(key_exists) > new_value)
			ft_strlcpy(key_exists, new_value, ft_strlen(new_value) + 1);
		else
		{
			new_key_and_value = malloc(ft_strlen(new_value) + ft_strlen(key) + 2);
			ft_strlcpy(new_key_and_value, key, ft_strlen(key) + 1);
			ft_strlcat(new_key_and_value, "=", 2);
			ft_strlcat(new_key_and_value, new_value, ft_strlen(new_value) + 1);
			return ;
		}
	}
	else if (t->env_size + 1 < t->env_capacity)
	{
		new_key_and_value = malloc(ft_strlen(new_value) + ft_strlen(key) + 2);
		ft_strlcpy(new_key_and_value, key, ft_strlen(key) + 1);
		ft_strlcat(new_key_and_value, "=", 2);
		ft_strlcat(new_key_and_value, new_value, ft_strlen(new_value) + 1);
		env[t->env_size] = new_key_and_value;
		env[t->env_size + 1] = NULL;
		t->env_size++;
		return ;
	}
	else
	{
		t->env_capacity *= 2;
		ft_realloc(env, t->env_size, t->env_capacity);
		new_key_and_value = malloc(ft_strlen(new_value) + ft_strlen(key) + 2);
		ft_strlcpy(new_key_and_value, key, ft_strlen(key) + 1);
		ft_strlcat(new_key_and_value, "=", 2);
		ft_strlcat(new_key_and_value, new_value, ft_strlen(new_value) + 1);
		env[t->env_size] = new_key_and_value;
		env[t->env_size + 1] = NULL;
		t->env_size++;
	}
}
