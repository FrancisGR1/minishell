/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:35 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/14 12:21:48 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir	*new_redir(t_string *args, t_string r_ptr)
{
	int		i;
	t_redir	*redir;

	if (!args)
		return (NULL);
	i = 0;
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	while (args[i].s && args[i].s < r_ptr.s)
		i++;
	redir->fd = args[i];
	return (redir);
}

bool	define_redir_type(t_redir *redir, t_string r_ptr)
{
	if (!r_ptr.s)
		return (false);
	if (*r_ptr.s == LESS && *(r_ptr.s + 1) == LESS)
	{
		redir->type = REDIR_HEREDOC;
	}
	else if (*r_ptr.s == MORE && *(r_ptr.s + 1) == MORE)
	{
		redir->type = REDIR_APPEND;
	}
	else if (*r_ptr.s == LESS)
	{
		redir->type = REDIR_INPUT;
	}
	else if (*r_ptr.s == MORE)
	{
		redir->type = REDIR_OUTPUT;
	}
	else
		return (false);
	return (true);
}

bool	get_redir(t_parser_buffer *pb, t_cmd *current_cmd, int *redir_idx)
{
	t_redir	*redir;

	pb->redir_ptr = &(((t_string *)pb->redir_ptrs->data)[*redir_idx]);
	redir = new_redir(pb->args_ptr, *pb->redir_ptr);
	if (!redir || !pb->redir_ptr)
		return (freen((void *)&redir), false);
	if (!define_redir_type(redir, *pb->redir_ptr))
		return (freen((void *)&redir), false);
	if (redir->type == REDIR_HEREDOC)
	{
		current_cmd->ri.has_heredoc = true;
		write_heredoc_path(current_cmd->ri.heredoc_file, rand_string());
	}
	if (redir->type == REDIR_INPUT || redir->type == REDIR_HEREDOC)
		current_cmd->ri.li_ptr = redir;
	if (redir->type == REDIR_OUTPUT || redir->type == REDIR_APPEND)
		current_cmd->ri.lo_ptr = redir;
	if (redir->type == REDIR_HEREDOC || redir->type == REDIR_APPEND)
		(*redir_idx)++;
	if (!redir || !redir->fd.s)
		return (free(redir), false);
	ft_lstadd_back(&current_cmd->redirs, ft_lstnew(redir));
	return (true);
}

void	remove_redirections(t_parser_buffer *pb, t_cmd *current_cmd)
{
	size_t		i;
	size_t		argc;
	size_t		nbytes;
	t_string	r_ptr;

	if (!pb || !current_cmd || !pb->args_ptr)
		return ;
	i = 0;
	r_ptr = ((t_string *)(pb->redir_ptrs->data))[pb->redir_idx];
	while (pb->args_ptr[i].s && pb->args_ptr[i].s < r_ptr.s)
		i++;
	argc = 0;
	while (pb->args_ptr->s && pb->args_ptr[argc].s)
		argc++;
	if (argc > 0)
		argc--;
	nbytes = (argc + 1 - i) * (sizeof(t_string));
	ft_memmove(&pb->args_ptr[i], &pb->args_ptr[i + 1], nbytes);
	pb->args_ptr[argc].s = NULL;
}

void	write_heredoc_path(char dest[], char *src)
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
}
