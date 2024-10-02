#include "minishell.h"

char ***get_args(t_cmd *cmds, size_t size);
static void free_args(char ***args);

int exec(t_cmd *cmds, t_terminal *t)
{
	const int command_c = t->cmds_num - 1; //mudar nome "last command para algo mais intuitivo
	int fds[command_c][2];
	char ***args = get_args(cmds, t->cmds_num);
	int i;
	int j;
	pid_t pid;

	i = -1;
	while (++i < command_c)
		pipe(fds[i]);
	i = 0;
	while (cmds[i].binary.s)
	{
		pid = fork();
		if (pid == CHILD)
		{
			if (i == 0)
				dup2(fds[i][1], OUT);
			else if (i == command_c)
				dup2(fds[i - 1][0], IN);
			else
			{
				dup2(fds[i - 1][0], IN);
				dup2(fds[i][1], OUT);
			}
			j = -1;
			while (++j < command_c)
			{
				close(fds[j][0]);
				close(fds[j][1]);
			}
			execvp(args[i][0], args[i]); //substituir por execve
		}
		i++;
	}
	j = -1;
	while (++j < command_c)
	{
		close(fds[j][0]);
		close(fds[j][1]);
	}
	while (wait(NULL) > 0)
		;
	free_args(args);
	return (0);
}

char ***get_args(t_cmd *cmds, size_t size)
{
	size_t i;
	size_t j;
	size_t k;
	char ***classic_strs;

	if (!cmds || !cmds->args || !cmds->args->s)
		return (NULL);
	classic_strs = malloc((size + 1) * sizeof(char **));
	i = 0;
	while (i < size)
	{
		j = 0;
		while (cmds[i].args[j].s)
			j++;
		char **cmd_args = malloc(sizeof(char *) * (j + 1));
		k = 0;
		while (k < j)
		{
			cmd_args[k] = ft_strndup(cmds[i].args[k].s, cmds[i].args[k].len);
			k++;
		}
		cmd_args[k] = NULL;
		classic_strs[i] = cmd_args;
		i++;
	}
	classic_strs[i] = NULL;
	return (classic_strs);
}

static void free_args(char ***args)
{
	int i;
	int j;

	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			free(args[i][j]);
			j++;
		}
		free(args[i]);
		i++;
	}
	free(args);
}
