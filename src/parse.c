#include "minishell.h"

static void substitute_seps(char *cmd, bool inside_quotes);
static int handle_quotes(t_string *s);
static void write_path(char dest[], char *src);
static char *rand_string(void);

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
	if (quote)
	{
		ft_fprintf(ERROR, "Format error: Quotes unclosed\n");
		t->exit_code = 2;
		return (NULL);
	}
	cmds = malloc((t->cmds_num + 1) * sizeof(t_cmd));
	pipe_sides = string_split(input, "\1");
	if (!pipe_sides)
	{
		free(cmds);
		free(pipe_sides);
		//não é erro. Só não tem input
		t->exit_code = 0;
		return (NULL);
	}
	idx = 0;
	while (idx < t->cmds_num)
	{
		t_dynamic_array *redir_ptrs = string_findall(pipe_sides[idx], "\2\3");
		t_string *args_ptr = string_split(pipe_sides[idx], DELIMITERS);
		if (!args_ptr)
		{
			free(cmds);
			free(pipe_sides);
			darr_free(redir_ptrs);
			free(args_ptr);
			t->exit_code = 2;
			ft_fprintf(ERROR, "Format error: Missing command\n");
			return (NULL);
		}
		size_t tmp_n = 0;
		while (args_ptr && args_ptr[tmp_n].s)
			tmp_n++;
		size_t last_idx = 0;
		if (tmp_n > 0)
			last_idx = tmp_n - 1;
		i = 0;
		cmds[idx].redirs = NULL;
		cmds[idx].has_heredoc = false;
		//Redireções
		while (redir_ptrs && i < redir_ptrs->len)
		{
			t_string ptr = ((t_string *)redir_ptrs->data)[i];
			size_t tmp_idx = 0;
			t_redir *redir = malloc(sizeof(t_redir));
			while (args_ptr[tmp_idx].s && args_ptr[tmp_idx].s < ptr.s)
				tmp_idx++;
			redir->fd = args_ptr[tmp_idx];
			if (!redir->fd.s)
			{
				free(pipe_sides);
				darr_free(redir_ptrs);
				free(args_ptr);
				free(redir);
				for (int i = idx; i >= 0; i--)
					ft_lstclear(&cmds[i].redirs, free);
				free(cmds);
				ft_fprintf(ERROR, "Format error: No redirection file\n");
				t->exit_code = 2;
				return (NULL);

			}
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
			ft_lstadd_back(&cmds[idx].redirs, ft_lstnew(redir));
			ft_memmove(&args_ptr[tmp_idx], &args_ptr[tmp_idx + 1], (last_idx + 1 - tmp_idx) * (sizeof(t_string)));
			args_ptr[last_idx--].s = NULL;
			i++;
		}
		if (cmds[idx].last_input_ptr && cmds[idx].last_input_ptr->type != REDIR_HEREDOC)
			cmds[idx].heredoc_file[0] = '\0';
		else
		{
			write_path(cmds[idx].heredoc_file, rand_string());
			printf("%s\n", cmds[idx].heredoc_file);
		}
		cmds[idx].binary = args_ptr[0];
		cmds[idx].args = args_ptr;
		handle_quotes(cmds[idx].args);
		//AFAZER: expandir aqui
		//NOTA: posso tornar o handle quotes e a expansão num só loop
		//
		darr_free(redir_ptrs);
		if (!cmds[idx].binary.s)
		{
			free(pipe_sides);
			free(args_ptr);
			for (int i = idx; i >= 0; i--)
				ft_lstclear(&cmds[i].redirs, free);
			free(cmds);
			ft_fprintf(ERROR, "Format error: No command\n");
			t->exit_code = 2;
			return (NULL);
		}
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


static void write_path(char dest[], char *src)
{
	const char *current_path = (const char *)getcwd(NULL, 0);
	const size_t current_path_size = ft_strlen(current_path);
	const size_t src_size = ft_strlen(src);
	const size_t total_size = current_path_size + src_size + 1;

	if (!current_path || total_size >= PATH_MAX ||
		ft_strlcpy(dest, current_path, current_path_size + 1) == 0 )
	{
		dest[0] = '\0';
		return ;
	}
	ft_strlcat(dest, "/", current_path_size + 2);
	ft_strlcat(dest, src, current_path_size + 2 + src_size);
	return ;
}

int ft_rand(void) 
{
	static unsigned long next = 1;
	next = next * 1103515245 + 12345;
	return((unsigned)(next/65536) % 32768);
}

//Esta função é horrível mas para os meus propósitos é mais do que suficiente
static char *rand_string(void)
{
	static char rs[21];
	int i;

	i = 0;
	while (i < 30)
		rs[i++] = ft_rand() % 25 + 97;
	rs[i] = '\0';
	return (rs);
}
