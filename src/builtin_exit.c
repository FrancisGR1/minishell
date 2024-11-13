/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 00:20:14 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:14 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static bool	str_is_num(char *s);

int	builtin_exit(char **argv, int argc, t_terminal *t)
{
	long	exit_code;

	if (argc == 1)
	{
		freexit(GENERAL_ERROR, t);
	}
	if (argc > 2)
	{
		ft_fprintf(ERROR, "exit: too many arguments\n");
		return (GENERAL_ERROR);
	}
	exit_code = ft_atoi_long(argv[1]);
	if (!str_is_num(argv[1]) || (exit_code > INT_MAX || exit_code < INT_MIN))
	{
		ft_fprintf(ERROR, "exit: %s: numeric argument required\n", argv[1]);
		freexit(BUILTIN_ERROR, t);
	}
	freexit(exit_code % 256, t);
	return (0);
}

static bool	str_is_num(char *s)
{
	if (!s)
		return (false);
	while (*s && ft_isdigit(*s))
		s++;
	return (*s == '\0');
}
