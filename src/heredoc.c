#include "minishell.h"

//https://stackoverflow.com/questions/70672456/how-here-document-works-in-shell
int heredoc(char *delimiter, bool is_last_redir, int terminal_fd)
{
	char *input;
	int should_write;
	int write_fd;
	int read_fd;
	//char buf[4000]; //substituir por alocação dinâmica

	input = NULL;
	should_write = 1;
	if (is_last_redir)
		write_fd = open("tmp", O_TRUNC | O_WRONLY);
	while (should_write)
	{
		ft_fprintf(terminal_fd, "> ");
		input = get_next_line(terminal_fd);
		should_write = ft_strncmp(input, delimiter, ft_strlen(input));
		if (should_write && is_last_redir)
		{
			write(write_fd, input, ft_strlen(input));
			write(write_fd, "\n", 1);
		}
		free(input);
	}
	if (is_last_redir)
	{
		close(write_fd);
		read_fd = open("tmp", O_RDONLY);
		dup2(read_fd, STDIN);
		close(read_fd);
	}
	//printf("exiting heredoc\n");
	return (0);
}
