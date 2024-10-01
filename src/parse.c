#include "minishell.h"

static void substitute_seps(char *cmd);


//echo todo > output > out2 > out3
//fila= out3, out2 

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
			printf("last idx %zu\n", last_idx);
			t_string ptr = ((t_string *)ptrs->data)[i];
			size_t tmp_idx = 0;
			t_redir redir;
			while (tmp[tmp_idx].s && tmp[tmp_idx].s < ptr.s)
			{
				tmp_idx++;
			}
			redir.file = tmp[tmp_idx]; //trim (remover espaços iniciais)
			redir.type = *ptr.s == '\2' ? INPUT : TRUNC;
			ft_fprintf(OUT, "redir file: %S\nredir type: %s\n", redir.file, redir.type == INPUT ? "<" : ">");
			q_push(&cmds[idx].redirs, &redir);
			int l = 0;
			printf("before:\n");
			while (tmp[l].s)
				ft_fprintf(OUT, "\t%S\n", tmp[l++]);
			ft_memmove(&tmp[tmp_idx], &tmp[tmp_idx + 1], (last_idx + 1 - tmp_idx) * (sizeof(t_string)));
			tmp[last_idx--].s = NULL;
			printf("after:\n");
			l = 0;
			while (tmp[l].s)
				ft_fprintf(OUT, "\t%S\n", tmp[l++]);
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
