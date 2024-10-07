#include "minishell.h"

static t_terminal *init_term(void);
static void reset_term(t_terminal **t);

int main(void)
{ 

	//AFAZER: verificar nº de argumentos
	t_cmd *cmds;
	char *input;
	t_string input_cpy;
	t_terminal *t;

	t = init_term();
	int i = 0;
	while (i < 3)
	{
		input = readline("mini> ");
		input_cpy = cstr_to_str(input);
		add_history(input);
		free(input);
		cmds = parse(input_cpy, t);
		exec(cmds, t);
		t->cmds = cmds;
		reset_term(&t);
		freen((void *)&input_cpy.s);
		i++;
		break ; //temporário para verificar leaks sem ter de apanhar sinais
	}
	close(t->terminal_fd);
	free(t);
}
static t_terminal *init_term(void)
{
	t_terminal *t;

	//AFAZER: libertar
	t = malloc(sizeof(t_terminal));
	t->cmds = NULL;
	t->cmds_num = 1;
	t->terminal_fd = dup(STDOUT);
	return (t);
}

static void reset_term(t_terminal **t)
{
	size_t i;

	if (!t || !*t)
		return ;
	//libertar comandos
	if ((*t)->cmds)
	{
		i = 0;
		while (i < (*t)->cmds_num)
		{
			free((*t)->cmds[i].args);
			q_destroy(&(*t)->cmds[i].redirs, NULL);
			i++;
		}
		free((*t)->cmds);
		(*t)->cmds = NULL;
	}
	(*t)->cmds_num = 1;
}

void debug_fds(const char *message) 
{
    char proc_path[64];
    char target[1024];
    printf("\n=== %s ===\n", message);
    
    // Print info for FDs 0-9 (adjust range as needed)
    for (int fd = 0; fd < 10; fd++) 
    {
        snprintf(proc_path, sizeof(proc_path), "/proc/self/fd/%d", fd);
        ssize_t len = readlink(proc_path, target, sizeof(target) - 1);
        if (len != -1) 
	{
            target[len] = '\0';
            printf("FD %d -> %s\n", fd, target);
        }
    }
    printf("\n");
}


