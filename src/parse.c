#include "minishell.h"

static void substitute_seps(char *cmd);

t_cmd *parse(t_string input, t_terminal *t)
{
	const t_string delimiters = new_str(DELIMITERS);
	const t_string delimiters_pipe = new_str("\1");
	size_t idx;
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
	pipe_sides = string_split(input, delimiters_pipe);
	idx = 0;
	while (idx < t->cmds_num)
	{
		t_string *tmp = string_split(pipe_sides[idx], delimiters);
		cmds[idx].binary = tmp[0];
		cmds[idx].args = tmp;
		idx++;
	}
	cmds[idx].binary = new_str(NULL);
	free(pipe_sides);
	free(delimiters_pipe.s);
	free(delimiters.s);
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
