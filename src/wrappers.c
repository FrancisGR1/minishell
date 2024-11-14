/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:49 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/14 18:39:33 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	save_signal(t_terminal *t)
{
	t->exit_code = FATAL_ERROR + g_sig_received;
	g_sig_received = 0;
}

t_string	ft_readline(char *prompt, t_terminal *t)
{
	char		*tmp;
	t_string	input;

	while (true)
	{
		tmp = readline(prompt);
		if (!tmp)
		{
			ft_fprintf(STDOUT, "exit\n");
			return (new_str(NULL, 0));
		}
		input = cstr_to_str(tmp);
		freen((void *)&tmp);
		if (g_sig_received || (int)input.len == str_iter(input, 0, input.len,
				ft_isspace))
		{
			string_free(&input);
			if (g_sig_received)
				save_signal(t);
			continue ;
		}
		add_history(input.s);
		return (input);
	}
}

void	safe_close(int fd_to_close)
{
	if (fd_to_close < 0)
		return ;
	close(fd_to_close);
}

void	safe_dup2(int fd, int duplicate_to)
{
	if (fd < 0 || duplicate_to < 0)
		return ;
	if (dup2(fd, duplicate_to) == -1)
		perror("dup2");
}
