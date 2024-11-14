/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:17 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	hd_write_to_file(int write_fd, char *delimiter);
static void	hd_change_stdin(char *heredoc_file, bool is_last_input,
				int terminal_fd);
static void	hd_set_temporary_std(int *saved_out, int *saved_in,
				int terminal_fd);
static void	hd_restore_previous_piped_fds(int saved_out, int saved_in);

int	heredoc(char *delimiter, char *heredoc_file, int terminal_fd,
		bool is_last_input)
{
	int	write_fd;
	int	exit_status;
	int	saved_out;
	int	saved_in;

	write_fd = open(heredoc_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (write_fd == -1)
		return (FILE_ERROR);
	hd_set_temporary_std(&saved_out, &saved_in, terminal_fd);
	exit_status = hd_write_to_file(write_fd, delimiter);
	hd_restore_previous_piped_fds(saved_out, saved_in);
	if (write_fd > 0)
		safe_close(write_fd);
	hd_change_stdin(heredoc_file, is_last_input, terminal_fd);
	return (exit_status);
}

static int	hd_write_to_file(int write_fd, char *delimiter)
{
	char	*input;

	while (true)
	{
		input = readline("> ");
		if (!input || ft_strcmp(input, delimiter) == 0 || g_sig_received)
		{
			freen((void *)&input);
			if (g_sig_received)
				return (g_sig_received);
			break ;
		}
		ft_putendl_fd(input, write_fd);
		freen((void *)&input);
		g_sig_received = 0;
	}
	return (EXIT_SUCCESS);
}

static void	hd_change_stdin(char *heredoc_file, bool is_last_input,
		int terminal_fd)
{
	int	read_fd;

	read_fd = open(heredoc_file, O_RDONLY);
	if (is_last_input)
	{
		safe_dup2(terminal_fd, STDIN);
		safe_dup2(read_fd, STDIN);
	}
	safe_close(read_fd);
}

static void	hd_set_temporary_std(int *saved_out, int *saved_in, int terminal_fd)
{
	*saved_out = dup(STDOUT);
	*saved_in = dup(STDIN);
	safe_dup2(terminal_fd, STDIN);
	safe_dup2(terminal_fd, STDOUT);
}

static void	hd_restore_previous_piped_fds(int saved_out, int saved_in)
{
	safe_dup2(saved_in, STDIN);
	safe_dup2(saved_out, STDOUT);
	safe_close(saved_in);
	safe_close(saved_out);
}
