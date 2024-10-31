/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:35 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	heredoc(char *delimiter, char *heredoc_file, int terminal_fd, bool is_last_input)
{
	char	*input;
	int		write_fd;
	int		read_fd;
	int		status;

	input = NULL;
	write_fd = open(heredoc_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (write_fd == -1)
		return (FILE_ERROR);
	//gerar stdout temporário (para o prompt do readline aparecer)
	int saved_out = dup(STDOUT);
	int saved_in = dup(STDIN);
	dup2(terminal_fd, STDIN);
	dup2(terminal_fd, STDOUT);
	status = EXIT_SUCCESS;
	while (true)
	{
		//TODO: meter a funcionar com pipes
		input = readline("> ");
		if (!input || ft_strcmp(input, delimiter) == 0 || (g_sig_received && ft_strlen(input) == 0))
		{
			freen((void *)&input);
			if (g_sig_received)
				status = g_sig_received;
			break ;
		}
		ft_putendl_fd(input, write_fd);
		freen((void *)&input);
		g_sig_received = 0;
	}
	//eliminar stdout temporário
	dup2(saved_in, STDIN);
	dup2(saved_out, STDOUT);
	close(saved_in);
	close(saved_out);
	//abrir ficheiro temporário para onde se vai mandar o output do heredoc
	read_fd = open(heredoc_file, O_RDONLY);
	if (write_fd > 0)
		close(write_fd);
	if (is_last_input)
	{
		dup2(terminal_fd, STDIN);
		dup2(read_fd, STDIN);
	}
	close(read_fd);
	return (status);
}

void wait_heredoc(int *hd_exit_status, pid_t pid)
{
	int wstatus;

	waitpid(pid, &wstatus, 0);
	if (WIFSIGNALED(wstatus))
	{
		*hd_exit_status = FATAL_ERROR + WTERMSIG(wstatus);
	}
	else  if (WIFEXITED(wstatus))
	{
		*hd_exit_status = WEXITSTATUS(wstatus);
	}
}
