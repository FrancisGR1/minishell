#include "minishell.h"

static char **get_sorted_env(t_terminal *t);
static void print_export(t_terminal *t);
static bool is_valid_identifier(char *arg);
static void save_var(char *arg, t_terminal *t);

int builtin_export(char **argv, int argc, t_terminal *t)
{
	int exit_code;

	if (!t || !t->env || !argv)
		return (EXIT_SUCCESS);
	if (argc == 1 || (!ft_isalnum(argv[1][0])))
	{
		print_export(t);
		return (EXIT_SUCCESS);
	}
	exit_code = EXIT_SUCCESS;
	while (*++argv)
	{
		if (!is_valid_identifier(*argv))
		{
			ft_fprintf(ERROR, "export: Invalid identifier: %s\n", *argv);
			exit_code = GENERAL_ERROR;
			continue ;
		}
		save_var(*argv, t);

	}
	return (exit_code);
}

static void save_var(char *arg, t_terminal *t)
{
	size_t key_len;
	char *key;
	char *val;

	if (!arg)
		return;
	key_len = ft_len_until(arg, '=');
	key = ft_strndup(arg, key_len);
	if (key_len  == ft_strlen(arg))
		val = ft_strdup("\0");
	else
		val = ft_strdup(arg + key_len + 1);
	env_set(t->env, key, val, t);
	freen((void *)&key);
	freen((void *)&val);

}

static char **get_sorted_env(t_terminal *t)
{
	char **sorted_env;
	char *tmp;
	size_t i;
	size_t j;

	if (!t || !t->env)
		return (NULL);
	sorted_env = env_dup(t->env, t);
	i = 0;
	while (sorted_env[i])
	{
		j = i;
		while (sorted_env[j])
		{
			if (ft_strcmp(sorted_env[i], sorted_env[j]) > 0)
			{
				tmp = sorted_env[j];
				sorted_env[j] = sorted_env[i];
				sorted_env[i] = tmp;
			}
			j++;
		}
		i++;
	}
	return (sorted_env);
}

static void print_export(t_terminal *t)
{
	size_t i;
	char *val;
	char *key;
	size_t key_len;
	char **env_tmp;

	if (!t || !t->env)
		return ;
	env_tmp = get_sorted_env(t);
	i = 0;
	while (env_tmp[i])
	{
		key_len = ft_len_until(env_tmp[i], '=');
		key = ft_strndup(env_tmp[i], key_len);
		if (key_len == ft_strlen(env_tmp[i]))
			val = ft_strdup("\0");
		else
			val = ft_strdup(env_tmp[i] + key_len + 1);
		ft_fprintf(STDOUT, "declare -x %s=\"%s\"\n", key, val);
		free(key);
		free(val);
		free(env_tmp[i]);
		i++;
	}
	free(env_tmp);
}

static bool is_valid_identifier(char *arg)
{
	if (!arg || (!ft_isalpha(*arg) && *arg != '_'))
		return (false);
	while(*arg)
	{
		if (!ft_isalpha(*arg) && *arg != '_')
			break ;
		arg++;
	}
	return (*arg  == '=' || *arg == '\0');
}
