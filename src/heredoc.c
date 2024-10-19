#include "minishell.h"

void heredoc(char *delimiter, char *heredoc_file, bool *open_error, int terminal_fd)
{
	char *input;
	int write_fd;
	int read_fd;

	input = NULL;
	write_fd = open(heredoc_file,  O_WRONLY | O_TRUNC | O_CREAT); 
	dup2(terminal_fd, STDIN);
	if (write_fd == -1)
		*open_error = true;
	while (true && !*open_error)
	{
		input = readline(HEREDOC_PROMPT);
		if (!input || ft_strncmp(input, delimiter, ft_strlen(input)) == 0)
		{
			freen((void *)&input);
			break ;
		}
		ft_putendl_fd(input, write_fd);
		freen((void *)&input);
	}
	freen((void *)&input);
	close(write_fd);
	read_fd = open(heredoc_file, O_RDONLY);
	dup2(read_fd, STDIN);
	close(read_fd);
}
