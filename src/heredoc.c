#include "minishell.h"

int heredoc(char *delimiter, bool is_last_redir, int terminal_fd)
{
	char *input;
	int should_write;
	int write_fd;
	int read_fd;

	input = NULL;
	should_write = 1;
	if (is_last_redir)
		write_fd = open("tmp", O_TRUNC | O_WRONLY);
	while (should_write)
	{
		ft_fprintf(terminal_fd, "> ");
		input = get_next_line(terminal_fd);
		if (!input)
			break ;
		should_write = ft_strncmp(input, delimiter, ft_strlen(input));
		if (should_write && is_last_redir)
		{
			write(write_fd, input, ft_strlen(input));
			write(write_fd, "\n", 1);
		}
		freen((void *)&input);
	}
	if (is_last_redir)
	{
		close(write_fd);
		read_fd = open("tmp", O_RDONLY);
		dup2(read_fd, STDIN);
		close(read_fd);
	}
	return (0);
}
