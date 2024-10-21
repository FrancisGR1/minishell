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
	dup2(terminal_fd, STDIN);
	if (write_fd == -1) //TODO: mudar isto
		*open_error = true;
	while (true && !*open_error)
	{
		input = readline(HEREDOC_PROMPT);
		if (!input || ft_strncmp(input, delimiter, ft_strlen(input)) == 0)
		{
			freen((void *)&input);
			break ;
		}
		ft_putendl_fd(input, write_fd);
		freen((void *)&input);
	}
	close(write_fd);
	read_fd = open(heredoc_file, O_RDONLY);
	if (is_last_input)
		dup2(read_fd, STDOUT);
	close(read_fd);
}

void	write_path(char dest[], char *src)
{
	const char		*current_path = (const char *)getcwd(NULL, 0);
	const size_t	current_path_size = ft_strlen(current_path);
	const size_t	src_size = ft_strlen(src);
	const size_t	total_size = current_path_size + src_size + 1;

	if (!current_path || total_size >= PATH_MAX || ft_strlcpy(dest,
				current_path, current_path_size + 1) == 0)
	{
		dest[0] = '\0';
		free((char *)current_path);
		return ;
	}
	free((char *)current_path);
	ft_strlcat(dest, "/", current_path_size + 2);
	ft_strlcat(dest, src, current_path_size + 2 + src_size);
	return ;
}
