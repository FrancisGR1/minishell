#include "minishell.h"

static char ***get_args(t_cmd *cmds, size_t size);
static int	set_redirs(t_queue *redirs);
static void free_args(char ***args);


void segv(int)
{
	printf("child segfaulted\n");
	exit(0);
}
int exec(t_cmd *cmds, t_terminal *t)
{
	const int command_c = t->cmds_num - 1;
	int fds[t->cmds_num][2];
	char ***args = get_args(cmds, t->cmds_num);
	int i;
	int j;
	pid_t pid;

	i = -1;
	while (++i < command_c)
		pipe(fds[i]);
	i = 0;
	while (cmds[i].binary.s)
	{
		pid = fork();
		if (pid == CHILD)
		{
			//AFAZER: estabelecer sinais aqui
			signal(SIGSEGV, segv); //temporário para detetar segfaults
			if (i == 0)
				dup2(fds[i][PIPE_WRITE], STDOUT);
			else if (i == command_c)
				dup2(fds[i - 1][PIPE_READ], STDIN);
			else
			{
				dup2(fds[i - 1][PIPE_READ], STDIN);
				dup2(fds[i][PIPE_WRITE], STDOUT);
			}
			set_redirs(cmds[i].redirs);
			j = -1;
			while (++j < command_c)
			{
				close(fds[j][0]);
				close(fds[j][1]);
			}
			execvp(args[i][0], args[i]); //substituir por execve
		}
		i++;
	}
	j = -1;
	while (++j < command_c)
	{
		close(fds[j][0]);
		close(fds[j][1]);
	}
	while (wait(NULL) > 0)
		;
	free_args(args);
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

static int	set_redirs(t_queue *redirs)
{
	t_redir r;
	int	redir_fd;
	char	file_name[FILENAME_MAX];

	while (!q_is_empty(redirs))
	{
		r = *(t_redir *)q_pop(&redirs);
		ft_strlcpy(file_name, r.fd.s, r.fd.len + 1);
		if (r.type == REDIR_INPUT)
		{
			printf("input from: %s\n", file_name);
			redir_fd = open(file_name, O_RDONLY);
			dup2(redir_fd, STDIN);
			close(redir_fd);
		}
		else if (r.type == REDIR_OUTPUT)
		{
			redir_fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC);
			dup2(redir_fd, STDOUT);
			close(redir_fd);
		}
		else if (r.type == REDIR_APPEND)
		{
			redir_fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND);
			dup2(redir_fd, STDOUT);
			close(redir_fd);
		}
		else if (r.type == REDIR_HEREDOC)
			printf("TODO <<\n");
		else //isto pode acontecer?
		{printf("error\n");}/*dá error*/;
	}
	return (0);
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
