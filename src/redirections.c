#include "minishell.h"

int	set_redirs(t_list *redirs, char *heredoc_file, int terminal_fd)
{
	t_redir *r;
	bool open_error;
	char	file_name[FILENAME_MAX];

	open_error = false;
	while (redirs)
	{
		r = (t_redir *)redirs->content;
		ft_strlcpy(file_name, r->fd.s, r->fd.len + 1);
		if (r->type == REDIR_INPUT)
			open_and_redirect(file_name, O_RDONLY, STDIN, &open_error);
		else if (r->type == REDIR_HEREDOC)
			heredoc(file_name, heredoc_file, &open_error, terminal_fd);
		else if (r->type == REDIR_OUTPUT)
			open_and_redirect(file_name, O_WRONLY | O_CREAT | O_TRUNC, 
					STDOUT, &open_error);
		else if (r->type == REDIR_APPEND)
			open_and_redirect(file_name, O_WRONLY | O_CREAT | O_APPEND,
					STDOUT, &open_error);
		redirs = redirs->next;
		debug_fds("\n------\n");
	}
	if (open_error)
		return (redir_error(r, heredoc_file, file_name));
	return (1);
}

void open_and_redirect(char *file, int flags, int fd_from, bool *open_error)
{
	int redir_fd;

	if (fd_from == STDOUT)
		redir_fd = open(file, flags, 0644);
	else
		redir_fd = open(file, flags);
	if (redir_fd == - 1)
		*open_error = true;
	else
	{
		*open_error = false;
		dup2(redir_fd, fd_from);
		close(redir_fd);
	}
}

int redir_error(t_redir *r, char *heredoc_file, char *file_name)
{
	if (r->type == REDIR_HEREDOC)
		perror(heredoc_file);
	else
		perror(file_name);
	return (0);
}

