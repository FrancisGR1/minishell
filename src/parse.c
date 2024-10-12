#include "minishell.h"

static void substitute_seps(char *cmd, bool inside_quotes);
static int handle_quotes(t_string *s);

t_cmd *parse(t_string input, t_terminal *t)
{
	size_t idx;
	size_t i;
	t_cmd *cmds;
	t_string *pipe_sides;
	char quote;

	idx = 0;
	quote = '\0';
	while (idx < input.len)
	{
		if (!quote && (input.s[idx] == '\'' || input.s[idx] == '\"'))
			quote = input.s[idx];
		else if (input.s[idx] == quote)
			quote = '\0';
		substitute_seps(&input.s[idx], quote);
		if (input.s[idx] == PIPE)
			t->cmds_num++;
		idx++;
	}
	cmds = malloc((t->cmds_num + 1) * sizeof(t_cmd));
	pipe_sides = string_split(input, "\1");
	idx = 0;
	while (idx < t->cmds_num)
	{
		t_dynamic_array *ptrs = string_findall(pipe_sides[idx], "\2\3");
		t_string *args_ptr = string_split(pipe_sides[idx], DELIMITERS);
		size_t tmp_n = 0;
		while (args_ptr && args_ptr[tmp_n].s)
			tmp_n++;
		size_t last_idx = 0;
		if (tmp_n > 0)
			last_idx = tmp_n - 1;
		i = 0;
		cmds[idx].redirs = NULL;
		cmds[idx].has_heredoc = false;
		//trata das redireções
		while (ptrs && i < ptrs->len)
		{
			t_string ptr = ((t_string *)ptrs->data)[i];
			size_t tmp_idx = 0;
			//como é que raio isto está a dar leaks?????
			t_redir *redir = malloc(sizeof(t_redir));
			printf("stored: %p\n", (void *)redir);
			printf("redir alloced: %p\n", redir);
			while (args_ptr[tmp_idx].s && args_ptr[tmp_idx].s < ptr.s)
				tmp_idx++;
			redir->fd = args_ptr[tmp_idx];
			if (*ptr.s == '\2' && *(ptr.s + 1) == '\2')
			{
				redir->type = REDIR_HEREDOC;
				cmds[idx].last_input_ptr = redir;
				cmds[idx].has_heredoc = true;
				i++;
			}
			else if (*ptr.s == '\3' && *(ptr.s + 1) == '\3')
			{
				redir->type = REDIR_APPEND;
				i++;
			}
			else if (*ptr.s == '\2')
			{
				redir->type = REDIR_INPUT;
				cmds[idx].last_input_ptr = redir;
			}
			else if (*ptr.s == '\3')
				redir->type = REDIR_OUTPUT;
			else
				redir->type = -1;
			printf("redir pushing..\n");
			ft_lstadd_back(&cmds[idx].redirs, ft_lstnew(redir));
			ft_memmove(&args_ptr[tmp_idx], &args_ptr[tmp_idx + 1], (last_idx + 1 - tmp_idx) * (sizeof(t_string)));
			args_ptr[last_idx--].s = NULL;
			i++;
		}
		cmds[idx].binary = args_ptr[0];
		cmds[idx].args = args_ptr;
		handle_quotes(cmds[idx].args);
		//AFAZER: expandir aqui
		//NOTA: posso tornar o handle quotes e a expansão num só loop
		//
		//printing debug de argumentos:
		//for (int i = 0; cmds[i].binary.s; i++)
		//	for (int j = 0; cmds[i].args[j].s; j++)
		//		ft_fprintf(STDOUT, "%S\n", cmds[i].args[j]);
		darr_free(ptrs);
		idx++;
	}
	cmds[idx].binary = new_str(NULL);
	free(pipe_sides);
	return (cmds);
}

static int handle_quotes(t_string *args)
{
	size_t i;
	size_t j;
	size_t k;
	char quote;

	i = 0;
	while (args[i].s)
	{
		j = 0;
		k = 0;
		quote = '\0';
		while (j < args[i].len)
		{
			if (!quote && (args[i].s[j] == '\'' || args[i].s[j] == '\"'))
			{
				quote = args[i].s[j];
			}
			else if (quote && args[i].s[j] == quote)
			{
				quote = '\0';
			}
			else
			{
				args[i].s[k] = args[i].s[j];
				k++;
			}
			j++;
		}
		args[i].len -= j - k;
		args[i].end -= j - k;
		i++;
	}
	return (0);
}

static void substitute_seps(char *cmd, bool inside_quotes)
{
	if (!cmd || inside_quotes)
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
