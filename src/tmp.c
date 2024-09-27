#include "minishell.h"
int my_exec(char ***args)
{
	int fds[2][2];

	pipe(fds[0]);
	pipe(fds[1]);

	int idx = 0;
	while (args[idx])
	{
		pid_t pid = fork();

		if (pid == CHILD)
		{
			if (idx == 0) //PRIMEIRO
			{
				printf("%s -> %d --> %d\n", args[idx][0], OUT, fds[0][0]);
				dup2(fds[0][1], OUT);
				close(fds[0][0]);
				close(fds[1][0]);
				close(fds[1][1]);
				close(fds[0][1]);
			}
			else if (idx == 1) //MEIO
			{
				printf("%s -> %d --> %d, %d --> %d\n", args[idx][0], IN, fds[0][0], OUT, fds[1][1]);
				dup2(fds[0][0], IN);
				dup2(fds[1][1], OUT);
				close(fds[1][0]);
				close(fds[0][1]);
				close(fds[0][0]);
				close(fds[1][1]);

			}
			else		//ÚLTIMO
			{
				printf("%s -> %d --> %d\n", args[idx][0], IN, fds[1][0]);
				dup2(fds[1][0], IN);
				close(fds[0][0]);
				close(fds[0][1]);
				close(fds[1][1]);
				close(fds[1][0]);
			};
			execvp(args[idx][0], args[idx]);
		}
		idx++;
	}
	int j = 0;
	while (j < 2)
	{
		close(fds[j][0]);
		close(fds[j][1]);
		j++;
	}
}



int main (void)
{
	char *cat1[] = {"cat", "tmp.c", NULL};
	char *cat2[] = {"rev", NULL, NULL};
	char *ls[] = {"wc", "-l", NULL};
	char **args[] = {cat1, cat2, ls, NULL};
	//char *cat[] = {"cat", "tmp.c", NULL};
	//char *grep[] = {"grep", "NULL", NULL};
	//char *wc[] = {"wc", "-l", NULL};
	//char **args[] = {cat, grep, wc, NULL};
	my_exec(args);
}
