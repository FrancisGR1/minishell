/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 00:20:15 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/13 00:20:15 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_unset(char **argv, int argc, t_terminal *t)
{
	int		var_to_delete_idx;
	char	*var_to_delete_ptr;
	size_t	offset;

	if (argc == 1)
		return (EXIT_SUCCESS);
	while (*++argv)
	{
		var_to_delete_idx = env_get_idx(t->env, *argv);
		if (var_to_delete_idx > -1)
		{
			offset = sizeof(char *) * (t->env_size - var_to_delete_idx);
			var_to_delete_ptr = t->env[var_to_delete_idx];
			if (offset > 0)
				ft_memmove(&t->env[var_to_delete_idx], &t->env[var_to_delete_idx
					+ 1], offset);
			freen((void *)&var_to_delete_ptr);
			t->env_size--;
		}
	}
	return (EXIT_SUCCESS);
}
