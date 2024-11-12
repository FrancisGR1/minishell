#include "minishell.h"

static bool is_flag(char *str);
//
//TODO: flag não estã a funcionar?
int builtin_echo(char **argv, int argc)
{
	bool should_print_newline;
	size_t i;

	if (argc == 1)
	{
		ft_fprintf(STDOUT, "\n");
		return (EXIT_SUCCESS);
	}
	i = 1;
	should_print_newline = true;
	if (is_flag(argv[i]))
		should_print_newline = false;
	while (is_flag(argv[i]))
		i++;
	while (argv[i])
	{
		ft_fprintf(STDOUT, "%s ", argv[i]);
		i++;
	}
	if (should_print_newline)
		ft_fprintf(STDOUT, "\n");
	return (EXIT_SUCCESS);
}

static bool is_flag(char *str)
{
	if (!str)
		return (false);
	if (*str == '-')
		str++;
	while (*str && *str == 'n')
	{
		str++;
	}
	return (*str == '\0');
}
