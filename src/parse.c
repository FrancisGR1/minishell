#include "minishell.h"

static void substitute_seps(char *cmd);

t_cmd *parse(t_string input, t_terminal *t)
{
	size_t idx;
	size_t i;
	t_cmd *cmds;
	t_string *pipe_sides;

	idx = 0;
	while (idx < input.len)
	{
		substitute_seps(&input.s[idx]);
		if (input.s[idx] == PIPE)
			t->cmds_num++;
		idx++;
	}
	cmds = malloc((t->cmds_num + 1) * sizeof(t_cmd));
	cmds->redirs = NULL;
	pipe_sides = string_split(input, "\1");
	idx = 0;
	while (idx < t->cmds_num)
	{
		t_dynamic_array *ptrs = string_findall(pipe_sides[idx], "\2\3");
		t_string *tmp = string_split(pipe_sides[idx], DELIMITERS);
		size_t tmp_n = 0;
		while (tmp[tmp_n].s)
			tmp_n++;
		size_t last_idx = tmp_n - 1;
		i = 0;
		while (i < ptrs->len)
		{
			t_string ptr = ((t_string *)ptrs->data)[i];
			size_t tmp_idx = 0;
			t_redir *redir = malloc(sizeof(t_redir));
			while (tmp[tmp_idx].s && tmp[tmp_idx].s < ptr.s)
			{
				tmp_idx++;
			}
			redir->fd = tmp[tmp_idx]; //trim (remover espaços iniciais)
			if (*ptr.s == '\2' && *(ptr.s + 1) == '\2')
			{
				redir->type = REDIR_HEREDOC;
				i++;
			}
			else if (*ptr.s == '\3' && *(ptr.s + 1) == '\3')
			{
				redir->type = REDIR_APPEND;
				i++;
			}
			else if (*ptr.s == '\2')
				redir->type = REDIR_INPUT;
			else if (*ptr.s == '\3')
				redir->type = REDIR_OUTPUT;
			else
				redir->type = -1;
			q_push(&cmds[idx].redirs, redir);
			ft_memmove(&tmp[tmp_idx], &tmp[tmp_idx + 1], (last_idx + 1 - tmp_idx) * (sizeof(t_string)));
			tmp[last_idx--].s = NULL;
			i++;
		}
		cmds[idx].binary = tmp[0];
		cmds[idx].args = tmp;
		darr_free(ptrs);
		idx++;
	}
	cmds[idx].binary = new_str(NULL);
	free(pipe_sides);
	return (cmds);
}


static void substitute_seps(char *cmd)
{
	if (!cmd)
		return ;
	if (*cmd == '<')
		*cmd = LESS;
	else if (*cmd == '>')
		*cmd = MORE;
	else if (*cmd == '|')
		*cmd = PIPE;
	else if (*cmd == ' ')
		*cmd = SPACE;
	return ;
}
