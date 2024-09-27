#include "minishell.h"

char ***get_args(t_cmd *cmds, size_t size);

// 0  1    2  3    4  5 
//[0, 1], [0, 1], [0, 1]
int exec(t_cmd *cmds, t_terminal *t)
{
	int fds[2 * t->cmds_num];
	const int last_command = t->cmds_num - 1;
	char ***args = get_args(cmds, t->cmds_num);
	int i;
	int j;
	pid_t pid;

	i = -1;
	while (++i < last_command)
		pipe(fds + i * 2);
	i = 0;
	while (cmds[i].binary.s)
	{
		pid = fork();
		if (pid == CHILD)
		{
			if (i == 0)
				dup2(fds[i * 2 + 1], OUT);
			else if (i == last_command)
				dup2(fds[i * 2 - 2], IN);
			else
			{
				dup2(fds[i * 2 - 2], IN);
				dup2(fds[i * 2 + 1], OUT);
			}
			j = 0;
			while (j < (2 * last_command))
				close(fds[j++]);
			execvp(args[i][0], args[i]); //substituir por execve
		}
		i++;
	}
	j = 0;
	while (j < (2 * last_command))
		close(fds[j++]);
	while (wait(NULL) > 0)
		;
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


