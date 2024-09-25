#include "minishell.h"
static const char **strings_to_classic_strs(t_string *strings);

//múltiplos pipes?
//https://stackoverflow.com/questions/8389033/implementation-of-multiple-pipes-in-c

//int exec(t_cmd *cmds)
//{
//	int fd[2][2];
//	char binary[400];
//	const char **args_0 = strings_to_classic_strs(cmds[0].args);
//	const char **args_1 = strings_to_classic_strs(cmds[1].args);
//	const char **args_2 = strings_to_classic_strs(cmds[2].args);
//
//	pipe(fd[0]);
//	pipe(fd[1]);
//	pid_t pid_1;
//	pid_1 = fork();
//	if (pid_1 == 0)
//	{
//		close(fd[0][0]);
//		close(fd[1][0]);
//		close(fd[1][1]);
//		dup2(fd[0][1], STDOUT_FILENO);
//		close(fd[0][1]);
//		ft_strlcpy(binary, cmds[0].binary.s, cmds[0].binary.len + 1);
//		execvp(binary, (char **)args_0);
//		perror("execve");
//		exit(EXIT_FAILURE);
//	}
//	pid_t pid_2;
//	pid_2 = fork();
//	if (pid_2 == 0)
//	{
//		close(fd[0][1]);
//		close(fd[1][0]);
//		dup2(fd[0][0], STDIN_FILENO);
//		dup2(fd[1][1], STDOUT_FILENO);
//		close(fd[0][0]);
//		close(fd[1][1]);
//		ft_strlcpy(binary, cmds[1].binary.s, cmds[1].binary.len + 1);
//		execvp(binary, (char **)args_1);
//		perror("execve");
//		exit(EXIT_FAILURE);
//	}
//	pid_t pid_3;
//	pid_3 = fork();
//	if (pid_3 == 0)
//	{
//		close(fd[0][0]);
//		close(fd[0][1]);
//		close(fd[1][1]);
//		dup2(fd[1][0], STDIN_FILENO);
//		close(fd[1][0]);
//		ft_strlcpy(binary, cmds[2].binary.s, cmds[2].binary.len + 1);
//		execvp(binary, (char **)args_2);
//		perror("execve");
//		exit(EXIT_FAILURE);
//	}
//	close(fd[0][0]);
//	close(fd[0][1]);
//	close(fd[1][0]);
//	close(fd[1][1]);
//	waitpid(pid_1, NULL, 0);
//	waitpid(pid_2, NULL, 0);
//	waitpid(pid_3, NULL, 0);
//	freen_arr((void **) args_0);
//	freen_arr((void **) args_1);
//	freen_arr((void **) args_2);
//	args_0 = NULL;
//	args_1 = NULL;
//	args_2 = NULL;
//	return (EXIT_SUCCESS);
//}
//p={0,1}
//{p,p,p,p}

int exec(t_cmd *cmds)
{
	int fd[2];
	int idx;
	int fdd;
	pid_t pid;

	fdd = 0;
	idx = 0;
	while (cmds[idx].binary.s)
	{
		pipe(fd);
		pid = fork();
		if (pid == CHILD)
		{
			dup2(fdd, 0);
			if (cmds[idx + 1].binary.s != NULL)
				dup2(fd[1], 1);
			close(fd[0]);
			char bin[400];
			ft_strlcpy(bin, cmds[idx].binary.s, cmds[idx].binary.len + 1);
			const char **args = strings_to_classic_strs(cmds[idx].args);
			execvp(bin, (char **)args);
		}
		else
		{
			wait(NULL);
			close(fd[1]);
			fdd = fd[0];
			idx++;
		}

	}
	return (0);
}
static const char **strings_to_classic_strs(t_string *strings)
{
	size_t idx;
	size_t total;
	const char **classic_strs;

	if (!strings || !strings->s)
		return (NULL);
	idx = 0;
	total = 0;
	while (strings[idx++].s)
		total++;
	classic_strs = malloc((total + 1) * sizeof(char *));
	idx = 0;
	while (idx < total)
	{
		classic_strs[idx] = ft_strndup(strings[idx].s, strings[idx].len);
		idx++;
	}
	classic_strs[idx] = NULL;
	return (classic_strs);
}
