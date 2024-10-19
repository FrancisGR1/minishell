#include "minishell.h"

bool format_args(t_parser_buffer *pb, t_cmd *cmds, int *redir_idx);
void init_redirs(t_parser_buffer *pb, size_t idx);
void init_parser(t_parser_buffer *pb, t_terminal *t);
bool set_cmd(t_cmd *cmds, size_t idx, t_string *args_ptr);
void *free_on_error(int exit_code, char *error_message, t_parser_buffer *pb);
int mark_special_characters(t_string input, size_t *cmds_num);
void remove_redirections(t_parser_buffer *pb, t_cmd *cmds);
bool get_redir(t_parser_buffer *pb, t_cmd *cmds, int *redir_idx);
void define_redir_type(t_redir *redir, t_string r_ptr, t_cmd *cmds, int *redir_idx);
t_redir *new_redir(t_string *args, t_string r_ptr);
static int remove_quotes(t_string arg);

//retirar do ficheiro
int ft_rand(void);
static char *rand_string(void);
static void write_path(char dest[], char *src);

t_cmd *parse(t_string input, t_terminal *t)
{
	t_parser_buffer pb;

	init_parser(&pb, t);
	if (!!mark_special_characters(input, &t->cmds_num))
		return (free_on_error(WRONG_FORMAT, "Format error: Quotes unclosed", &pb));
	pb.cmds = malloc((t->cmds_num + 1) * sizeof(t_cmd));
	pb.pipe_sides = string_split(input, "\1");
	if (!pb.pipe_sides)
		return (free_on_error(NO_INPUT, NULL, &pb));
	while (++pb.idx < (int) t->cmds_num)
	{
		pb.args_ptr = string_split(pb.pipe_sides[pb.idx], DELIMITERS);
		if (!pb.args_ptr)
			return (free_on_error(WRONG_FORMAT, "Format error: Missing command", &pb));
		init_redirs(&pb, pb.idx);
		while (pb.redir_ptrs && ++pb.redir_idx < (int) pb.redir_ptrs->len)
			if (!format_args(&pb, pb.cmds, &pb.redir_idx))
				return (free_on_error(WRONG_FORMAT, "Format error: No redirection file", &pb));
		if (!set_cmd(pb.cmds, pb.idx, pb.args_ptr))
			return (free_on_error(WRONG_FORMAT, "Format error: No command", &pb));
		darr_free(pb.redir_ptrs);
	}
	pb.cmds[pb.idx].binary = new_str(NULL);
	free(pb.pipe_sides);
	return (pb.cmds);
}

static int remove_quotes(t_string arg)
{
	size_t j;
	size_t k;
	char quote;

	j = 0;
	k = 0;
	quote = '\0';
	while (j < arg.len)
	{
		if (!quote && (arg.s[j] == '\'' || arg.s[j] == '\"'))
			quote = arg.s[j];
		else if (quote && arg.s[j] == quote)
			quote = '\0';
		else
		{
			arg.s[k] = arg.s[j];
			k++;
		}
		j++;
	}
	arg.len -= j - k;
	arg.end -= j - k;
	return (0);
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
		free((char *)current_path);
		return ;
	}
	free((char *)current_path);
	ft_strlcat(dest, "/", current_path_size + 2);
	ft_strlcat(dest, src, current_path_size + 2 + src_size);
	return ;
}

//Esta função é horrível mas para os meus propósitos é mais do que suficiente
int ft_rand(void) 
{
	static unsigned long next = 1;
	next = next * 1103515245 + 12345;
	return((unsigned)(next/65536) % 32768);
}

static char *rand_string(void)
{
	static char rs[20];
	int i;

	i = 0;
	while (i < 20)
		rs[i++] = ft_rand() % 25 + 97;
	rs[i] = '\0';
	return (rs);
}

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
	//printf("redir info:\n\tinfo: %s\n\tsize: %zu\n", pb->redir_ptr->s, pb->redir_ptr->len);
	redir = new_redir(pb->args_ptr, *pb->redir_ptr);
	define_redir_type(redir, *pb->redir_ptr, cmds, redir_idx);
	//printf("redir info 2:\n\tdelimiter: %s\n\ttype: %d\n", redir->fd.s, redir->type);
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
	//printf("debugging:\n");
	//debug_args(pb->cmds, pb->t->cmds_num);
}

int mark_special_characters(t_string input, size_t *cmds_num)
{
	char quote;
	size_t i;

	quote = '\0';
	i = 0;
	while (i < input.len)
	{
		if (!quote && (input.s[i] == '\'' || input.s[i] == '\"'))
			quote = input.s[i];
		else if (input.s[i] == quote)
			quote = '\0';
		if (!quote && input.s[i] == '<')
			input.s[i] = LESS;
		else if (!quote && input.s[i] == '>')
			input.s[i] = MORE;
		else if (!quote && input.s[i] == '|')
			input.s[i] = PIPE;
		else if (!quote && input.s[i] == ' ')
			input.s[i] = SPACE;
		if (!quote && input.s[i] == PIPE)
			(*cmds_num)++;
		i++;
	}
	return (quote);
}


void *free_on_error(int exit_code, char *error_message, t_parser_buffer *pb)
{
	int i;

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


bool set_cmd(t_cmd *cmds, size_t idx, t_string *args_ptr)
{
	size_t i;

	if (cmds[idx].last_input_ptr && cmds[idx].last_input_ptr->type == REDIR_HEREDOC)
	{
		write_path(cmds[idx].heredoc_file, rand_string());
	}
	cmds[idx].binary = args_ptr[0];
	cmds[idx].args = args_ptr;
	if (!cmds[idx].binary.s)
		return (false);
	i = 0;
	while (args_ptr[i].s)
	{
		remove_quotes(args_ptr[i]);
		//AFAZER: expandir aqui
		//expand(args_ptr[i]);
		i++;
	}
	return (true);
}

void init_parser(t_parser_buffer *pb, t_terminal *t)
{
	pb->pipe_sides = NULL;
	pb->args_ptr = NULL;
	pb->redir_ptrs = NULL;
	pb->idx = -1;
	pb->redir_idx = -1;
	pb->cmds = NULL;
	pb->t = t;
}

void init_redirs(t_parser_buffer *pb, size_t idx)
{
	pb->cmds[idx].redirs = NULL;
	pb->cmds[idx].last_input_ptr = NULL;
	pb->cmds[idx].has_heredoc = false;
	pb->redir_idx = -1;
	pb->redir_ptrs = string_findall(pb->pipe_sides[idx], "\2\3");
	pb->redir_ptr = NULL;
}

bool format_args(t_parser_buffer *pb, t_cmd *cmds, int *redir_idx)
{
	if (!get_redir(pb, cmds, redir_idx))
		return (false);
	if (cmds)
		remove_redirections(pb, cmds);
	//AFAZER: expandir aqui:
	return (true);
}
