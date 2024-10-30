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
	if (write_fd == -1)
		*open_error = true;
	//gerar stdout temporário
	while (true && !*open_error)
	{
		//TODO: meter a funcionar com pipes
		input = readline("> ");
		if (!input || ft_strcmp(input, delimiter) == 0 || (g_sig_received && ft_strlen(input) == 0))
		{
			freen((void *)&input);
			break ;
		}
		ft_putendl_fd(input, write_fd);
		freen((void *)&input);
		g_sig_received = 0;
	}
	if (write_fd > 0)
		close(write_fd);
	read_fd = open(heredoc_file, O_RDONLY);
	if (is_last_input)
	{
		dup2(terminal_fd, STDIN);
		dup2(read_fd, STDIN);
	}
	if (read_fd > 0)
		close(read_fd);
	//eliminar stdout temporário
}


//static void disable_ctrl_d(void)
//{
//	static termios term;
//
//	tcgetattr(STDIN_FILENO), &term);
//	term.c_lflag = &= ~ICANON;
//	term.c_cc[V_EOF] = 0;
//
//	tcsetattr(STDIN_FILENO, TCSNOW, &term);
//
//	tcsetattr(STDIN_FEILNEO, TCSNOW, &term);
//}
