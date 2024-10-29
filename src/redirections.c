/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:35 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	set_redirs(t_list *redirs, char *heredoc_file, int terminal_fd, t_redir *li_ptr, t_redir *lo_ptr)
{
	t_redir	*r;
	bool	open_error;
	char	file_name[FILENAME_MAX];

	open_error = false;
	while (redirs) 
	{
		r = (t_redir *)redirs->content;
		ft_strlcpy(file_name, r->fd.s, r->fd.len + 1);
		if (r->type == REDIR_INPUT)
			open_and_redirect_input(file_name, &open_error, terminal_fd, li_ptr == r);
		else if (r->type == REDIR_HEREDOC)
			heredoc(file_name, heredoc_file, &open_error, terminal_fd, li_ptr ==r);
		else if (r->type == REDIR_OUTPUT)
			open_and_redirect_output(file_name, O_WRONLY | O_CREAT | O_TRUNC, &open_error, terminal_fd, lo_ptr == r);
		else if (r->type == REDIR_APPEND)
			open_and_redirect_output(file_name, O_WRONLY | O_CREAT | O_APPEND, &open_error, terminal_fd, lo_ptr == r);
		if (open_error)
			return (redir_error(r, heredoc_file, file_name));
		redirs = redirs->next;
	}
	return (1);
}

void	open_and_redirect_input(char *file, bool *open_error, int terminal_fd, bool is_last_output)
{
	int	redir_fd;

	redir_fd = open(file, O_RDONLY);
	if (redir_fd == -1)
	{
		*open_error = true;
	}
	else if (is_last_output)
	{
		dup2(terminal_fd, STDIN);
		dup2(redir_fd, STDIN);
	}
	if (open_error == false)
		close(redir_fd);
}

void	open_and_redirect_output(char *file, int flags, bool *open_error, int terminal_fd, bool is_last_output)
{
	int	redir_fd;

	redir_fd = open(file, flags, DEFAULT_FILE_PERM);
	if (redir_fd == -1)
	{
		*open_error = true;
	}
	else if (is_last_output)
	{
		dup2(terminal_fd, STDOUT);
		dup2(redir_fd, STDOUT);
	}
	if (open_error == false)
		close(redir_fd);
}

int	redir_error(t_redir *r, char *heredoc_file, char *file_name)
{
	if (r->type == REDIR_HEREDOC)
	{
		ft_fprintf(STDERR, "heredoc error: ");
		perror(heredoc_file);
		ft_fprintf(STDERR, "heredoc file: %s\n", heredoc_file);
	}
	else
	{
		ft_fprintf(STDERR, "MY ERROR:\n");
		perror(file_name);
	}
	return (0);
}
