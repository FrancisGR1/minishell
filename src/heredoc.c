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

void	heredoc(char *delimiter, char *heredoc_file, bool *open_error,
		int terminal_fd, bool is_last_input)
{
	char	*input;
	int		write_fd;
	int		read_fd;

	input = NULL;
	write_fd = open(heredoc_file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (write_fd == -1) //TODO: mudar isto
		*open_error = true;
	while (true && !*open_error)
	{
		//TODO: usar termios para bloquear ctrl + \ e 
		//ctrl + d quando a linha está cheia
		ft_putstr_fd("> ", terminal_fd);
		//TODO: sai quando input é delimitador - 1 char?
		//ex: delim: EOF1
		//->sai quando: >EOF
		input = get_next_line(terminal_fd);
		if (!input || ft_strncmp(input, delimiter, ft_strlen(input)) == 0)
		{
			if (!input)
				ft_putstr_fd("\n", terminal_fd); 
			freen((void *)&input);
			break ;
		}
		ft_putendl_fd(input, write_fd);
		freen((void *)&input);
	}
	close(write_fd);
	read_fd = open(heredoc_file, O_RDONLY);
	if (is_last_input)
	{
		dup2(terminal_fd, STDIN);
		dup2(read_fd, STDIN);
	}
	close(read_fd);
}
