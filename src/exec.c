#include "minishell.h"
static char **strings_to_classic_strs(t_string *strings);

int *exec(t_string *cmd)
{
	pid_t pid;
	const char **args = (const char **)strings_to_classic_strs(cmd);

	pid = fork();
	if (pid == 0)
	{
		execvp(cmd->s, (char **)args);
		//Erro aqui:
		exit(EXIT_FAILURE);
	}
	else
	{
		wait(NULL);
	}
	return (EXIT_SUCCESS);
}

static char **strings_to_classic_strs(t_string *strings)
{
	size_t idx;
	size_t total;
	char **classic_strs;

	idx = 0;
	while (strings[idx++].s)
		total++;
	classic_strs = malloc((total + 1) * sizeof(char *));
	idx = 0;
	while (strings[idx].s)
	{
		classic_strs[idx] = ft_strndup(strings[idx].s, strings[idx].len);
		printf("args: %s\n", classic_strs[idx]);
		idx++;
	}
	classic_strs[idx] = NULL;
	return (classic_strs);
}
