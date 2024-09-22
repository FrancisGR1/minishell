#include "minishell.h"

static void substitute_seps(char *cmd);

t_string *parse(t_string cmd)
{
	const t_string delimiters = new_str(DELIMITERS);
	const t_string delimiters_no_spc = new_str(DELIMITERS_NO_SPC);
	size_t idx;
	t_string *cmds;
	t_darr *seps;

	idx = 0;
	while (idx < cmd.len)
		substitute_seps(&cmd.s[idx++]);
	cmds = string_split(cmd, delimiters);
	seps = string_findall(cmd, delimiters_no_spc);
	(void ) seps;
//	for (int i = 0; cmds[i].s; i++)
//		string_put(cmds[i], OUT);
//	for (size_t i = 0; i < seps->len; i++)
//		ft_fprintf(OUT, ":%d\n", ((int *)seps->data)[i]);
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
