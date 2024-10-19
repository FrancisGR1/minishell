#include "minishell.h"

static int	set_redirs(t_list *redirs, char *heredoc_file, int terminal_fd);
void free_cmd_args(t_cmd *cmds, int commands);
void close_fds(int fds[][2], int cmds_num);
void dup2_pipe(int fds[][2], int idx, int last);
void init_pipes(int fds[][2], int cmds_num);
void freexit(int exit_code, t_cmd *cmds, t_terminal *t);
int wait_subprocesses(pid_t *subprocesses, int commands, t_cmd *cmds);
void exec_subprocess(int fds[][2], t_cmd *cmds, int idx, t_terminal *t);
static void alloc_args(t_cmd *cmds, int size);
int redir_error(t_redir *r, char *heredoc_file, char *file_name);
static void open_and_redirect(char *file, int flags, int fd_from, bool *open_error);


int exec(t_cmd *cmds, t_terminal *t)
{
	const int command_c = t->cmds_num - 1;
	int fds[t->cmds_num][2];
	pid_t pids[t->cmds_num];
	int i;

	init_pipes(fds, command_c);
	i = 0;
	alloc_args(cmds, t->cmds_num);
	while (i < (int) t->cmds_num)
	{
		pids[i] = fork();
		if (pids[i] == SUBPROCESS)
		{
			signal(SIGSEGV, catch_subprocess_segv); //temporário para detetar segfaults
			exec_subprocess(fds, cmds, i, t);
		}
		if (cmds[i].has_heredoc)
		{
			waitpid(pids[i], 0, 0);
		}
		i++;
	}
	free_cmd_args(cmds, t->cmds_num - 1);
	close_fds(fds, command_c);
	return (wait_subprocesses(pids, t->cmds_num, cmds));
}

static int	set_redirs(t_list *redirs, char *heredoc_file, int terminal_fd)
{
	t_redir *r;
	bool open_error;
	char	file_name[FILENAME_MAX];

	open_error = false;
	while (redirs)
	{
		r = (t_redir *)redirs->content;
		ft_strlcpy(file_name, r->fd.s, r->fd.len + 1);
		if (r->type == REDIR_INPUT)
			open_and_redirect(file_name, O_RDONLY, STDIN, &open_error);
		else if (r->type == REDIR_HEREDOC)
			heredoc(file_name, heredoc_file, &open_error, terminal_fd);
		else if (r->type == REDIR_OUTPUT)
			open_and_redirect(file_name, O_WRONLY | O_CREAT | O_TRUNC, 
					STDOUT, &open_error);
		else if (r->type == REDIR_APPEND)
			open_and_redirect(file_name, O_WRONLY | O_CREAT | O_APPEND,
					STDOUT, &open_error);
		redirs = redirs->next;
		debug_fds("\n------\n");
	}
	if (open_error)
		return (redir_error(r, heredoc_file, file_name));
	return (1);
}

static void alloc_args(t_cmd *cmds, int size)
{
	int i;
	int j;
	int k;
	char **cmd_args;

	if (!cmds || !cmds->args || !cmds->args->s)
		return ;
	i = 0; 
	while (i < size)
	{
		j = 0;
		while (cmds[i].args[j].s)
			j++;
		cmd_args = malloc(sizeof(char *) * (j + 1));
		k = 0;
		while (k < j)
		{
			cmd_args[k] = string_convert_back(cmds[i].args[k]);
			k++;
		}
		cmd_args[k] = NULL;
		cmds[i].cstr_args = cmd_args;
		i++;
	}
}

void free_cmd_args(t_cmd *cmds, int commands)
{
	int i;
	int j;

	i = 0;
	while (i < commands)
	{
		j = 0;
		while (cmds[i].cstr_args[j])
		{
			free(cmds[i].cstr_args[j]);
			j++;
		}
		free(cmds[i].cstr_args);
		i++;
	}
}

void close_fds(int fds[][2], int cmds_num)
{
	int i;

	i = 0;
	while (i < cmds_num)
	{
		close(fds[i][0]);
		close(fds[i][1]);
		i++;
	}
}

void dup2_pipe(int fds[][2], int idx, int last)
{
	if (idx == 0)
		dup2(fds[idx][PIPE_WRITE], STDOUT);
	else if (idx == last)
	{
		dup2(fds[idx - 1][PIPE_READ], STDIN);
	}
	else
	{
		dup2(fds[idx - 1][PIPE_READ], STDIN);
		dup2(fds[idx][PIPE_WRITE], STDOUT);
	}
}

void freexit(int exit_code, t_cmd *cmds, t_terminal *t)
{

	close(t->terminal_fd);
	free_cmd_args(cmds, t->cmds_num - 1);
	string_free(&t->input);
	reset_term(&t);
	free(t);
	exit(exit_code);
}

int wait_subprocesses(pid_t *subprocesses, int commands, t_cmd *cmds)
{
	int exit_code;
	int wstatus;
	int i;

	exit_code = 0;
	wstatus = 0;
	i = 0;
	while (i < commands)
	{
		if (!cmds[i].has_heredoc)
			waitpid(subprocesses[i], &wstatus, 0);
		else
			unlink(cmds[i].heredoc_file);
		if (WIFEXITED(wstatus))
			exit_code = WEXITSTATUS(wstatus);
		i++;
	}
	return (exit_code);
}

void init_pipes(int fds[][2], int cmds_num)
{
	int i;

	i = 0;
	while (i < cmds_num)
	{
		pipe(fds[i]);
		i++;
	}
}


void exec_subprocess(int fds[][2], t_cmd *cmds, int idx, t_terminal *t)
{
	if (t->cmds_num != 1)
		dup2_pipe(fds, idx, t->cmds_num - 1);
	if (!set_redirs(cmds[idx].redirs, cmds[idx].heredoc_file, t->terminal_fd))
	{
		close_fds(fds, t->cmds_num - 1);
		freexit(FILE_ERROR, cmds, t);
	}
	close_fds(fds, t->cmds_num - 1);
	if (execvp(cmds[idx].cstr_args[0], cmds[idx].cstr_args) == -1) //substituir por execve
	{
		if (errno == ENOENT)
			ft_fprintf(ERROR, "%s: Command not found\n", cmds[idx].cstr_args[0]);
		else
			perror(cmds[idx].cstr_args[0]);
		freexit(CMD_NOT_FOUND, cmds, t);
	}
}

static void open_and_redirect(char *file, int flags, int fd_from, bool *open_error)
{
	int redir_fd;

	if (fd_from == STDOUT)
		redir_fd = open(file, flags, 0644);
	else
		redir_fd = open(file, flags);
	if (redir_fd == - 1)
		*open_error = true;
	else
	{
		*open_error = false;
		dup2(redir_fd, fd_from);
		close(redir_fd);
	}
}

int redir_error(t_redir *r, char *heredoc_file, char *file_name)
{
	if (r->type == REDIR_HEREDOC)
		perror(heredoc_file);
	else
		perror(file_name);
	return (0);
}
