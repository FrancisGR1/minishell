#include "minishell.h"
//https://stackoverflow.com/questions/70672456/how-here-document-works-in-shell
//
//<<EOF1 cat -> funciona
//<<EOF1 <<EOF2 cat -> no 2 sai pq o input é nulo
int heredoc(char *delimiter, int heredoc_fd, bool is_last_redir)
{
	char *input;
	bool should_write;
	(void) heredoc_fd;
	int write_fd;
	int read_fd;

	input = NULL;
	should_write = true;
	if (is_last_redir)
		write_fd = open("tmp", O_TRUNC | O_WRONLY);
	while (should_write)
	{
		ft_fprintf(STDOUT, "heredoc> ");
		input = get_next_line(STDIN);
		should_write = ft_strncmp(input, delimiter, ft_strlen(input));
		if (should_write && is_last_redir)
		{
			write(write_fd, input, ft_strlen(input));
			write(write_fd, "\n", 1);
		}
		free(input);
	}
	//abrimos um novo ficheiro para voltar ao inicio do texto
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
