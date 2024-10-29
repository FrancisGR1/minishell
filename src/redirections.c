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
			open_and_redirect(file_name, O_RDONLY, &open_error, terminal_fd, li_ptr == r);
		else if (r->type == REDIR_HEREDOC)
			heredoc(file_name, heredoc_file, &open_error, terminal_fd, li_ptr ==r);
		else if (r->type == REDIR_OUTPUT)
			open_and_redirect(file_name, O_WRONLY | O_CREAT | O_TRUNC, &open_error, terminal_fd, lo_ptr == r);
		else if (r->type == REDIR_APPEND)
			open_and_redirect(file_name, O_WRONLY | O_CREAT | O_APPEND, &open_error, terminal_fd, lo_ptr == r);
		if (open_error)//não sei se é suposto parar no erro 
			return (redir_error(r, heredoc_file, file_name));
		redirs = redirs->next;
	}
	return (1);
}

void	open_and_redirect(char *file, int flags, bool *open_error, int terminal_fd, bool is_last_output)
{
	int	redir_fd;

	if (flags == O_RDONLY)
		redir_fd = open(file, flags);
	else
		redir_fd = open(file, flags, 0644);
	if (redir_fd == -1)
		*open_error = true;
	else if (is_last_output)
	{
		if (flags == O_RDONLY)
		{
			dup2(terminal_fd, STDIN);
			dup2(redir_fd, STDIN);
		}
		else
		{
			dup2(terminal_fd, STDOUT);
			dup2(redir_fd, STDOUT);
		}
	}
	close(redir_fd);
	*open_error = false;
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
		perror(file_name);
	return (0);
}
