#include "minishell.h"

//https://stackoverflow.com/questions/70672456/how-here-document-works-in-shell
int heredoc(char *delimiter, bool is_last_redir, int *write_ptr, int terminal_fd)
{
	char *input;
	bool should_write;
	int write_fd;
	int read_fd;
	char buf[4000]; //substituir por alocação dinâmica

	input = NULL;
	should_write = true;
	if (is_last_redir)
		write_fd = open("tmp", O_TRUNC | O_WRONLY);
	dup2(terminal_fd, STDOUT);
	dup2(terminal_fd, STDIN);
	while (should_write)
	{
		ft_fprintf(STDOUT, "> ");
		input = get_next_line(STDIN);
		should_write = ft_strncmp(input, delimiter, ft_strlen(input));
		if (should_write && is_last_redir)
		{
			write(write_fd, input, ft_strlen(input));
			write(write_fd, "\n", 1);
		}
		free(input);
	}
	(void) write_ptr;
	(void) buf;
	//if (write_ptr)
	//{
	//	printf("writing to pipe\n");
	//	ssize_t bytes_read = 0;
	//	while ((bytes_read = read(write_fd, buf, sizeof(buf))) > 0)
	//		write(terminal_fd, buf, bytes_read);
	//	close(write_fd);
	//	
	//}	
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
