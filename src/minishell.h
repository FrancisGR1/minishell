#ifndef MINISHELL_H
# define MINISHELL_H

//Readline includes
#include <readline/readline.h>
#include <readline/history.h>
#include <readline/history.h>

//manipulate terminal output
#include <termios.h>

//wait() 
#include <sys/wait.h>
//open() 
#include <fcntl.h> 

//error handling
#include <errno.h>

//Our LIB
#include "../libft/libft.h"

//Variável global com último valor de sinal recebido 
extern int g_sig_received;

//Delimiter codes
#define PIPE '\1'
#define LESS '\2'
#define MORE '\3'
#undef SPACE
#define SPACE '\4'

#define DELIMITERS "\1\2\3\4"
#define DELIMITERS_NO_SPC "\1\2\3"

//Pipe
#define PIPE_WRITE 1
#define PIPE_READ 0

//Pid of subprocess spawned is 0
#define SUBPROCESS 0

//Main struct
typedef struct s_terminal t_terminal;

//All the data and meta-data of a commad
typedef struct s_command t_cmd;

enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
};

typedef struct s_redirections
{
	enum e_redir_type type;
	t_string fd;
}	t_redir;

struct s_command
{
	t_string binary;
	t_string *args;
	t_list *redirs;
	t_redir *last_input_ptr;
	bool has_heredoc;
};

struct s_terminal
{
	t_cmd *cmds;
	size_t cmds_num;
	int terminal_fd;
	int exit_code;
	t_string input_cpy;
};

//parse
t_cmd *parse(t_string input, t_terminal *t);

//execution
int exec(t_cmd *cmds, t_terminal *t);

//redirection: heredoc
int heredoc(char *delimiter, bool is_last_redir, int terminal_fd);

//main struct utils
void reset_term(t_terminal **t);

//debug utils
void debug_fds(const char *message);

//signals
void load_signals(void);
void load_signals_2();
void signals_handler(int signum, siginfo_t *inf, void *ctx);
void signals_handler_2(int signum, siginfo_t *inf, void *ctx);
void load_signals_subprocess(void);
void signals_handler_subprocess(int signum, siginfo_t *inf, void *ctx);

#endif /*MINISHELL_H*/
