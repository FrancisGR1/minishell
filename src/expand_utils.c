/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 20:03:48 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:15 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid_dollar_char(int c)
{
	static int	prev_char;
	bool		is_valid;

	is_valid = false;
	if ((ft_isalnum(c) || c == '?') && prev_char != '?')
		is_valid = true;
	prev_char = c;
	return (is_valid);
}

void	remove_empty_codes(t_string *str)
{
	size_t	i;
	size_t	j;

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (j < str->len)
	{
		if (str->s[j] == '\6' && str->len == 1)
		{
			str->s[i] = '\0';
		}
		else if (str->s[j] == '\6')
			;
		else
		{
			str->s[i] = str->s[j];
			i++;
		}
		j++;
	}
	str->len = i;
}
