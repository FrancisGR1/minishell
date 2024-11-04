/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:01:34 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:01:34 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	freexit(int exit_code, t_cmd *cmds, t_terminal *t)
{
	(void) cmds; /*TODO: eliminar*/
	close(t->terminal_fd);
	string_free(&t->input);
	reset_term(&t);
	destroy_term(&t);
	free(t);
	exit(exit_code);
}

void	*free_on_error(int exit_code, char *error_message, t_parser_buffer *pb)
{
	int	i;

	i = pb->idx;
	if (pb->pipe_sides)
		freen((void *)&pb->pipe_sides);
	if (pb->args_ptr)
		freen((void *)&pb->args_ptr);
	if (pb->redir_ptrs)
		darr_free(pb->redir_ptrs);
	while (i >= 0)
	{
		ft_lstclear(&pb->cmds[i].redirs, free);
		i--;
	}
	freen((void *)&pb->cmds);
	if (error_message)
		ft_fprintf(ERROR, "%s\n", error_message);
	pb->t->exit_code = exit_code;
	return (NULL);
}

void	free_cmd_args(t_cmd *current_cmd)
{
	size_t	i;

	i = 0;
	if (!current_cmd || !current_cmd->cstr_args)
		return ;
	while (i < current_cmd->argc)
	{
		//TODO: mudar isto
		string_free(&current_cmd->args[i]);
		if (i < current_cmd->cstr_argc)
			freen((void *)&current_cmd->cstr_args[i]);
		i++;
	}
	freen((void *)&current_cmd->args);
	freen((void *)&current_cmd->cstr_args);
}


//TODO: mudar função de sítio
void cleanup_arg(char *str)
{
	size_t i;
	size_t j;

	if (!str)
		return ;
	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == '\5' || str[i] == '\6')
		{
			j++;
			str[i] = str[j];
			continue;
		}
		else
		{
			str[i] = str[j];
			j++;
		}
		i++;
	}
	str[i] = '\0';
}

//TODO: mudar função de sítio
void	alloc_args(t_cmd *cmds, int commands_num, char **t_env)
{
	int		i;
	int		k;
	int		j;
	char	**cmd_args;

	if (!cmds || !cmds->args)
		return ;
	i = -1;
	while (++i < commands_num)
	{
		//TODO: acho que posso tirar isto daqui
		if (cmds[i].argc == 0)
		{
			cmds[i].cstr_args = NULL;
			continue ;
		}
		cmd_args = ft_calloc(sizeof(char *),  (cmds[i].argc + 1));
		k = -1;
		j = 0;
		while (++k < (int) cmds[i].argc)
		{
			if (ft_strcmp(cmds[i].args[k].s, EMPTY_EXPANDED_STR) == 0)
				continue ;
			else
			{
				cmd_args[j] = string_convert_back(cmds[i].args[k]);
				cleanup_arg(cmd_args[j]);
			}
			j++;
		}
		cmds[i].cstr_argc = j;
		cmd_args[j] = NULL;
		//encontrar o caminho
		//
		//TODO: verificar se é builtin antes de tentar caminho
		char *res;
		if (cmd_args[0] && cmd_args[0][0])
			res = find_path(cmd_args[0], t_env);
		else
			res = NULL;
		if (res)
		{
			free(cmd_args[0]);
			cmd_args[0] = res;
		}
		//----------------
		cmds[i].cstr_args = cmd_args;
		//TODO: isto tem de ser apanhado ao nível da execução 
		if (!cmds[i].cstr_args[0])
			perror("NO ARGS\n");
	}
}
