#include "minishell.h"
//https://stackoverflow.com/questions/70672456/how-here-document-works-in-shell
int heredoc(char *delimiter, int heredoc_fd)
{
	char *input;
	bool should_write;
	(void) heredoc_fd;
	static int original_stdin;

	if (!original_stdin)
		original_stdin = dup(STDIN);
	input = NULL;
	should_write = true;
	int write_fd = open("tmp", O_TRUNC | O_WRONLY);
	while (should_write)
	{
		ft_fprintf(STDOUT, "heredoc> ");
		input = get_next_line(original_stdin);
		if (!delimiter)
			printf("delim is null\n");
		if (!input)
			printf("input is null\n");
		should_write = ft_strncmp(input, delimiter, ft_strlen(input));
		if (should_write)
		{
			write(write_fd, input, ft_strlen(input));
			write(write_fd, "\n", 1);
		}
		free(input);
	}
	//abrimos um novo ficheiro para voltar ao inicio do texto
	close(write_fd);
	int read_fd = open("tmp", O_RDONLY);
	dup2(read_fd, original_stdin);
	close(read_fd);
	printf("exiting heredoc\n");
	return (0);
}
