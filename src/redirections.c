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
/* ************************************name************************************** */

#include "minishell.h"

int	set_redirs(t_list *redirs, char *heredoc_file, int terminal_fd, t_redir *li_ptr, t_redir *lo_ptr)
{
	t_redir	*r;
	int	redir_error;
	char	file_name[FILENAME_MAX];

	redir_error = 0;
	while (redirs && !redir_error) 
	{
		r = (t_redir *)redirs->content;
		ft_strlcpy(file_name, r->fd.s, r->fd.len + 1);
		if (r->type == REDIR_INPUT)
			redir_error = open_and_redirect_input(file_name, terminal_fd, li_ptr == r);
		else if (r->type == REDIR_HEREDOC)
			redir_error = heredoc(file_name, heredoc_file, terminal_fd, li_ptr ==r);
		else if (r->type == REDIR_OUTPUT)
			redir_error = open_and_redirect_output(file_name, O_WRONLY | O_CREAT | O_TRUNC, terminal_fd, lo_ptr == r);
		else if (r->type == REDIR_APPEND)
			redir_error = open_and_redirect_output(file_name, O_WRONLY | O_CREAT | O_APPEND, terminal_fd, lo_ptr == r);
		if (redir_error == 1)
			print_redir_error(redir_error, r, heredoc_file, file_name);
		redirs = redirs->next;
	}
	return (redir_error);
}

int	open_and_redirect_input(char *file, int terminal_fd, bool is_last_output)
{
	int	redir_fd;

	redir_fd = open(file, O_RDONLY);
	if (redir_fd == -1)
	{
		return (FILE_ERROR);
	}
	else if (is_last_output)
	{
		dup2(terminal_fd, STDIN);
		dup2(redir_fd, STDIN);
	}
	close(redir_fd);
	return (EXIT_SUCCESS);
}

int	open_and_redirect_output(char *file, int flags, int terminal_fd, bool is_last_output)
{
	int	redir_fd;

	redir_fd = open(file, flags, DEFAULT_FILE_PERM);
	if (redir_fd == -1)
	{
		return (FILE_ERROR);
	}
	else if (is_last_output)
	{
		dup2(terminal_fd, STDOUT);
		dup2(redir_fd, STDOUT);
	}
	close(redir_fd);
	return (EXIT_SUCCESS);
}

int	print_redir_error(int redir_error, t_redir *r, char *heredoc_file, char *file_name)
{
	if (r->type == REDIR_HEREDOC)
	{
		ft_fprintf(STDERR, "heredoc error: ");
		perror(heredoc_file);
		ft_fprintf(STDERR, "heredoc file: %s\n", heredoc_file);
	}
	else
	{
		perror(file_name);
	}
	return (redir_error);
}
