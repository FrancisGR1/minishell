#include "minishell.h"

int builtin_pwd(void)
{
	char cwd[PATH_MAX];

	if (getcwd(cwd, PATH_MAX) == NULL)
	{
		perror("pwd");
		return (BUILTIN_ERROR);
	}
	ft_fprintf(STDOUT, "%s\n", cwd);
	return (EXIT_SUCCESS);
}
