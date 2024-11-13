/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:47 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:15 by frmiguel         ###   ########.fr       */
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
	i = -1;
	while (env[++i])
	{
		if (i == capacity - 1)
		{
			capacity += ENV_INITIAL_SIZE;
			env_dup = (char **)ft_realloc(env_dup, i * sizeof(char *), capacity
					* sizeof(char *));
		}
		env_dup[i] = ft_strndup(env[i], ft_strlen(env[i]));
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
	while (*env)
	{
		equals_ptr = ft_strchr(*env, '=');
		if (equals_ptr && (size_t)(equals_ptr - *env) == key_len
			&& !ft_strncmp(*env, key, key_len))
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

static char	*make_var(char *new_value, char *key)
{
	char	*var;

	if (!new_value || !key)
		return (NULL);
	var = malloc(ft_strlen(new_value) + ft_strlen(key) + 2);
	if (!var)
		return (NULL);
	ft_strlcpy(var, key, ft_strlen(key) + 1);
	ft_strlcat(var, "=", 2 + ft_strlen(var));
	ft_strlcat(var, new_value, ft_strlen(new_value) + 2 + ft_strlen(var));
	return (var);
}

void	env_set(char **env, char *key, char *new_value, t_terminal *t)
{
	char	*new_key_and_value;
	int		key_idx;

	if (!t || !env || !key || !new_value)
		return ;
	key_idx = env_get_idx(env, key);
	new_key_and_value = make_var(new_value, key);
	if (key_idx > -1)
	{
		freen((void *)&env[key_idx]);
		env[key_idx] = new_key_and_value;
		return ;
	}
	if (t->env_size + 1 >= t->env_capacity)
	{
		t->env_capacity *= 2;
		t->env = (char **)ft_realloc(env, t->env_size * sizeof(char *),
				t->env_capacity * sizeof(char *));
		env = t->env;
	}
	env[t->env_size] = new_key_and_value;
	env[t->env_size + 1] = NULL;
	t->env_size++;
}

int	env_get_idx(char **env, char *key)
{
	size_t	key_len;
	char	*equals_ptr;
	size_t	idx;

	if (!env || !key)
		return (0);
	key_len = ft_strlen(key);
	if (!key_len)
		return (0);
	idx = 0;
	while (env[idx])
	{
		equals_ptr = ft_strchr(env[idx], '=');
		if (equals_ptr && (size_t)(equals_ptr - env[idx]) == key_len
			&& !ft_strncmp(env[idx], key, key_len))
		{
			break ;
		}
		idx++;
	}
	if (!env[idx])
		return (-1);
	return (idx);
}
