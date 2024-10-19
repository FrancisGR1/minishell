#include "minishell.h"

t_redir *new_redir(t_string *args, t_string r_ptr)
{
	int i;
	t_redir *redir;

	i = 0;
	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	while (args[i].s && args[i].s < r_ptr.s)
		i++;
	redir->fd = args[i];
	redir->location = i;
	return (redir);
}

void define_redir_type(t_redir *redir, t_string r_ptr, t_cmd *cmds, int *redir_idx)
{
	if (!r_ptr.s)
		return ;
	if (*r_ptr.s == LESS && *(r_ptr.s + 1) == LESS)
	{
		redir->type = REDIR_HEREDOC;
		cmds->last_input_ptr = redir;
		cmds->has_heredoc = true;
		(*redir_idx)++;
	}
	else if (*r_ptr.s == MORE && *(r_ptr.s + 1) == MORE)
	{
		redir->type = REDIR_APPEND;
		(*redir_idx)++;
	}
	else if (*r_ptr.s == LESS)
	{
		redir->type = REDIR_INPUT;
		cmds->last_input_ptr = redir;
	}
	else if (*r_ptr.s == MORE)
		redir->type = REDIR_OUTPUT;
	else
		redir->type = -1;
}


bool get_redir(t_parser_buffer *pb, t_cmd *cmds, int *redir_idx)
{
	t_redir *redir;

	pb->redir_ptr = &(((t_string *)pb->redir_ptrs->data)[*redir_idx]);
	redir = new_redir(pb->args_ptr, *pb->redir_ptr);
	define_redir_type(redir, *pb->redir_ptr, cmds, redir_idx);
	if (!redir || !redir->fd.s)
		return (false);
	ft_lstadd_back(&cmds->redirs, ft_lstnew(redir));
	return (true);
}

void remove_redirections(t_parser_buffer *pb, t_cmd *cmds)
{
	size_t i;
	size_t argc;
	size_t nbytes;
	t_string r_ptr;

	if (!pb || !cmds || !pb->args_ptr)
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

