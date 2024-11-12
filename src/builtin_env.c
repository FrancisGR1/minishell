#include "minishell.h"

int builtin_env(t_terminal *t)
{
	size_t i;
	const char **environment;

	if (!t || !t->env)
		return (EXIT_SUCCESS);
	environment = (const char **)t->env;
	i = 0;
	while (environment[i])
	{
		ft_puts(environment[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
