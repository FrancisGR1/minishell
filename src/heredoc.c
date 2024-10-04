#include "minishell.h"
//https://stackoverflow.com/questions/70672456/how-here-document-works-in-shell
int heredoc(char *delimiter)
{
	char *input;
	bool should_run;
	int	heredoc_fd[2];

	input = NULL;
	should_run = true;
	pipe(heredoc_fd);
	while (should_run)
	{
		ft_fprintf(STDOUT, "heredoc> ");
		input = get_next_line(STDIN);
		should_run = ft_strncmp(input, delimiter, ft_strlen(input));
		free(input);
	}
	printf("exiting\n");
	return (0);
}
