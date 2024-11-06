/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wrappers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:49 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:17 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_add_history(t_string input)
{
	if (g_sig_received)
		return ;
	if ((int)input.len == str_iter(input, 0, input.len, ft_isspace))
		return ;
	add_history(input.s);
}

t_string	ft_readline(char *prompt)
{
	char		*tmp;
	t_string	input;

	tmp = readline(prompt);
	input = cstr_to_str(tmp);
	freen((void *)&tmp);
	return (input);
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
