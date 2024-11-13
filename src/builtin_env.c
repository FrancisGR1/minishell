/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 00:20:14 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:14 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(t_terminal *t)
{
	size_t		i;
	const char	**environment;

	if (!t || !t->env)
		return (EXIT_SUCCESS);
	environment = (const char **)t->env;
	i = 0;
	while (environment[i])
	{
		ft_puts(environment[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
