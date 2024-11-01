/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:34 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:34 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

void	debug_cstr_args(t_cmd *cmds, size_t cmds_num)
{
	if (!cmds)
		ft_fprintf(ERROR, "cmds null\n");
	if (!cmds_num)
		ft_fprintf(ERROR, "cmds num: 0\n");
	if (!cmds->cstr_args)
		ft_fprintf(ERROR, "cstr arg null\n");
	for (size_t i = 0; i < cmds_num; ++i)
	{
		for (size_t j = 0; cmds[i].cstr_args[j]; ++j)
			ft_fprintf(STDOUT, "->%s\n", cmds[i].cstr_args[j]);
		ft_fprintf(STDOUT, "\n");
	}
}

void	debug_args(char *msg, t_string *args, int cmds_num)
{
	ft_fprintf(STDOUT, "=======%s=======\n", msg);
	if (!args)
		ft_fprintf(ERROR, "t_string *args null\n");
	ft_fprintf(STDOUT, "num of args: %d\n", cmds_num);
	for (int j = 0; j < cmds_num && args[j].s; ++j)
	{
		ft_fprintf(STDOUT, "->%S(%d)(%p)\n", args[j], args[j].type, args[j].s);
	}
	ft_fprintf(STDOUT, "\n");
}

void	debug_cmds(char *msg, t_cmd *cmds, size_t cmds_num)
{
	char allocated[] = "STR_ALLOCATED";
	char pointer[] = "STR_PTR";
	char null[] = "STR_NULL";
	if (!cmds)
		ft_fprintf(ERROR, "cmds null\n");
	if (!cmds_num)
		ft_fprintf(ERROR, "cmds num: 0\n");
	if (!cmds->args || cmds->argc == 0)
		ft_fprintf(ERROR, "args num: 0\n");
	ft_fprintf(STDOUT, "=======%s=======\n", msg);
	for (size_t i = 0; i < cmds_num; ++i)
	{
		for (size_t j = 0; j < cmds[i].argc; ++j)
		{
			char *str_type;
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
}

void	debug_redirections(t_cmd *cmds, size_t cmds_num)
{
	t_redir	*r;
	t_list	*redirs;

	if (!cmds)
		ft_fprintf(ERROR, "cmds null\n");
	if (!cmds_num)
		ft_fprintf(ERROR, "cmds num: 0\n");
	if (!cmds->redirs)
		ft_fprintf(ERROR, "no redirs\n");
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
}

void	catch_subprocess_segv(int n)
{
	(void)n;
	printf("subprocess segfaulted at pid %d\n", getpid());
	exit(0);
}
