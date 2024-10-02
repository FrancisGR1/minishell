#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	if (argc < 2)
		return (printf("More args\n"));
	int fd_out = open("output.md", O_CREAT | O_TRUNC | O_WRONLY);
	int fd_in = open("input.md", O_RDONLY);
	++argv;
	int id = fork();
	if (id == 0)
	{
		//input:
		dup2(fd_in, STDIN_FILENO);
		//output:
		dup2(fd_out, STDOUT_FILENO);
		close(fd_out);
		execvp(argv[0], argv);
	}
	close(fd_out);
	close(fd_in);
	while (wait(NULL) > 0)
		;
}

