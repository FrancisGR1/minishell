/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:34 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/06 21:57:14 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/*
#include "minishell.h"

void	debug_fds(const char *message, int fd)
{
	char	proc_path[64];
	char	target[1024];
	ssize_t	len;

	ft_fprintf(fd, "\n=== %s ===\n", message);
	for (int fd = 0; fd < 10; fd++)
	{
		snprintf(proc_path, sizeof(proc_path), "/proc/self/fd/%d", fd);
		len = readlink(proc_path, target, sizeof(target) - 1);
		if (len != -1)
		{
			target[len] = '\0';
			ft_fprintf(fd, "FD %d -> %s\n", fd, target);
		}
	}
	printf("\n");
}

int	debug_cstr_args(char *msg, t_cmd *cmds, int cmds_num)
{
	ft_fprintf(STDOUT, "=======%s=======\n", msg);
	if (!cmds)
		return (ft_fprintf(ERROR, "cmds null\n"));
	if (!cmds_num)
		return (ft_fprintf(ERROR, "cmds num: 0\n"));
	if (!cmds->cstr_args)
		return (ft_fprintf(ERROR, "cstr arg null\n"));
	for (int i = 0; i < cmds_num; ++i)
	{
		ft_fprintf(STDOUT, "argc=%d\n", cmds[i].cstr_argc);
		for (size_t j = 0; cmds[i].cstr_args[j]; ++j)
			ft_fprintf(STDOUT, "->%s\n", cmds[i].cstr_args[j]);
		ft_fprintf(STDOUT, "\n");
	}
	return (0);
}

int	debug_args(char *msg, t_string *args)
{
	ft_fprintf(STDOUT, "=======%s=======\n", msg);
	if (!args)
		return (ft_fprintf(ERROR, "t_string *args null\n"));
	ft_fprintf(STDOUT, "num of args: %d\n");
	for (int j = 0; args[j].s; ++j)
	{
		ft_fprintf(STDOUT, "->%S(%d)(%p)\n", args[j], args[j].type, args[j].s);
	}
	ft_fprintf(STDOUT, "\n");
	return (0);
}

int	debug_cmds(char *msg, t_cmd *cmds, size_t cmds_num)
{
	char	allocated[] = "STR_ALLOCATED";
	char	pointer[] = "STR_PTR";
	char	null[] = "STR_NULL";
	char	*str_type;

	if (!cmds)
		return (ft_fprintf(ERROR, "cmds null\n"));
	if (!cmds_num)
		return (ft_fprintf(ERROR, "cmds num: 0\n"));
	if (!cmds->args || cmds->argc == 0)
		return (ft_fprintf(ERROR, "args num: 0\n"));
	ft_fprintf(STDOUT, "=======%s=======\n", msg);
	for (size_t i = 0; i < cmds_num; ++i)
	{
		for (size_t j = 0; j < cmds[i].argc; ++j)
		{
			if (cmds[i].args[j].type == STR_POINTER)
				str_type = pointer;
			else if (cmds[i].args[j].type == STR_ALLOCATED)
				str_type = allocated;
			else
				str_type = null;
			ft_fprintf(STDOUT, "->%S (%s)\n", cmds[i].args[j], str_type);
		}
		ft_fprintf(STDOUT, "\n");
	}
	return (0);
}

int	debug_redirections(t_cmd *cmds, size_t cmds_num)
{
	t_redir	*r;
	t_list	*redirs;

	if (!cmds)
		return (ft_fprintf(ERROR, "cmds null\n"));
	if (!cmds_num)
		return (ft_fprintf(ERROR, "cmds num: 0\n"));
	if (!cmds->redirs)
		return (ft_fprintf(ERROR, "no redirs\n"));
	for (size_t i = 0; i < cmds_num; ++i)
	{
		redirs = cmds[i].redirs;
		while (redirs)
		{
			r = (t_redir *)redirs->content;
			ft_fprintf(STDOUT, "\n-----------------\ntype: %d\nstring: %S\n",
				r->type, r->fd);
			redirs = redirs->next;
		}
	}
	return (0);
}

void	catch_subprocess_segv(int n)
{
	(void)n;
	printf("subprocess segfaulted at pid %d\n", getpid());
	exit(EXIT_FAILURE);
}

*/
