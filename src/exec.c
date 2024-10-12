#include "minishell.h"

static char ***get_args(t_cmd *cmds, size_t size);
static int	set_redirs(t_queue *redirs, t_redir *last_input_ptr, int terminal_fd);
static void free_args(char ***args);


void segv(int nahnah)
{
	(void) nahnah;
	printf("subprocess segfaulted at pid %d\n", getpid());
	exit(0);
}

int exec(t_cmd *cmds, t_terminal *t)
{
	const int command_c = t->cmds_num - 1;
	int fds[t->cmds_num][2];
	pid_t subprocesses[t->cmds_num];
	char ***args = get_args(cmds, t->cmds_num);
	int i;
	int j;
	pid_t pid;

	i = -1;
	while (++i < command_c)
		pipe(fds[i]);
	//criacao do ficheiro temporario
	//nota: abordagem de implementacao rapida,
	//enrobustecer a criacao do ficheiro no futuro
	int out_fd = open("tmp", O_CREAT, 0600);  //substituir tmp por gerador de strings aleat
	i = 0;
	while (cmds[i].binary.s)
	{
		pid = fork();
		if (pid == SUBPROCESS)
		{
			signal(SIGSEGV, segv); //temporário para detetar segfaults
			if (i == 0)
			{
				dup2(fds[i][PIPE_WRITE], STDOUT);
			}
			else if (i == command_c)
			{
				dup2(fds[i - 1][PIPE_READ], STDIN);
			}
			else
			{
				dup2(fds[i - 1][PIPE_READ], STDIN);
				dup2(fds[i][PIPE_WRITE], STDOUT);
			}
			set_redirs(cmds[i].redirs, cmds[i].last_input_ptr, t->terminal_fd);
			j = -1;
			while (++j < command_c)
			{
				close(fds[j][0]);
				close(fds[j][1]);
			}
			//debug_fds(ft_itoa(getppid()));
			execvp(args[i][0], args[i]); //substituir por execve
		}
		subprocesses[i] = pid;
		if (cmds[i].has_heredoc)
		{
			waitpid(pid, 0, 0);
		}
		i++;
	}
	j = -1;
	while (++j < command_c)
	{
		close(fds[j][0]);
		close(fds[j][1]);
	}
	close(out_fd);
	i = 0;
	while (i < (int) t->cmds_num)
	{
		if (!cmds[i].has_heredoc)
			waitpid(subprocesses[i], 0, 0);
		i++;
	};
	unlink("tmp");
	free_args(args);
	return (0);
}

static int	set_redirs(t_queue *redirs, t_redir *last_input_ptr, int terminal_fd)
{
	t_redir *r;
	int	redir_fd;
	char	file_name[FILENAME_MAX];

	while (!q_is_empty(redirs))
	{
		r = (t_redir *)q_pop(&redirs);
		ft_strlcpy(file_name, r->fd.s, r->fd.len + 1);
		if (r->type == REDIR_INPUT)
		{
			redir_fd = open(file_name, O_RDONLY);
			dup2(redir_fd, STDIN);
			close(redir_fd);
		}
		else if (r->type == REDIR_HEREDOC)
			heredoc(file_name, last_input_ptr == r, terminal_fd);
		else if (r->type == REDIR_OUTPUT)
		{
			redir_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC);
			dup2(redir_fd, STDOUT);
			close(redir_fd);
		}
		else if (r->type == REDIR_APPEND)
		{
			redir_fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND);
			dup2(redir_fd, STDOUT);
			close(redir_fd);
		}
		else //teoricamente isto não deve acontecer
			ft_fprintf(ERROR, "error\n");
		printf("freeing: %p\n", r);
		free(r);
	}
	return (0);
}

static char ***get_args(t_cmd *cmds, size_t size)
{
	size_t i;
	size_t j;
	size_t k;
	char ***classic_strs;

	if (!cmds || !cmds->args || !cmds->args->s)
		return (NULL);
	classic_strs = malloc((size + 1) * sizeof(char **));
	i = 0;
	while (i < size)
	{
		j = 0;
		while (cmds[i].args[j].s)
			j++;
		char **cmd_args = malloc(sizeof(char *) * (j + 1));
		k = 0;
		while (k < j)
		{
			cmd_args[k] = ft_strndup(cmds[i].args[k].s, cmds[i].args[k].len);
			k++;
		}
		cmd_args[k] = NULL;
		classic_strs[i] = cmd_args;
		i++;
	}
	classic_strs[i] = NULL;
	return (classic_strs);
}

static void free_args(char ***args)
{
	int i;
	int j;

	i = 0;
	while (args[i])
	{
		j = 0;
		while (args[i][j])
		{
			free(args[i][j]);
			j++;
		}
		free(args[i]);
		i++;
	}
	free(args);
}
