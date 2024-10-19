#ifndef MINISHELL_H
# define MINISHELL_H

//our lib
#include "../libft/libft.h"

//Readline includes
#include <readline/readline.h>
#include <readline/history.h>
#include <readline/history.h>

//wait() 
#include <sys/wait.h>
//open() 
#include <fcntl.h> 

//error handling
#include <errno.h>

//global variable with last value received
extern int g_sig_received;

//special characters
#define PIPE '\1'
#define LESS '\2'
#define MORE '\3'
#undef SPACE
#define SPACE '\4'
#define DELIMITERS "\1\2\3\4"

//Prompts
#define RL_PROMPT "minishell> "
#define HEREDOC_PROMPT "> "

//Exit codes
#define NO_INPUT 0
#define FILE_ERROR 1
#define WRONG_FORMAT 2
#define CMD_NOT_FOUND 127
#define FATAL_ERROR 130

//Pipe ends
#define PIPE_WRITE 1
#define PIPE_READ 0

//Pid of subprocess spawned is 0
#define SUBPROCESS 0

//Main struct
typedef struct s_terminal t_terminal;

//All the data and meta-data of a commad
typedef struct s_command t_cmd;

//redirections
typedef struct s_redirections t_redir;

//Stores the temporary data for/from parsing and
//the important data for convenience
typedef struct s_parser_buffer
{
	t_string *pipe_sides;
	t_string *args_ptr;
	t_dynamic_array *redir_ptrs;
	int idx;
	int redir_idx;
	t_terminal *t;
	t_cmd *cmds;
	t_string *redir_ptr;
} t_parser_buffer;

enum e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
};

struct s_redirections
{
	enum e_redir_type type;
	t_string fd;
	int location;
};

struct s_command
{
	t_string binary;
	t_string *args;
	char **cstr_args;
	t_list *redirs;
	t_redir *last_input_ptr;
	bool has_heredoc;
	char heredoc_file[PATH_MAX];
};

struct s_terminal
{
	t_cmd *cmds;
	size_t cmds_num;
	int terminal_fd;
	int exit_code;
	t_string input;
};

//parse
t_cmd *parse(t_string input, t_terminal *t);

//execution
int exec(t_cmd *cmds, t_terminal *t);

//redirection: heredoc
void heredoc(char *delimiter, char *heredoc_file, bool *open_error, int terminal_fd);

//terminal struct utils
t_terminal *init_term(void);
void reset_term(t_terminal **t);
void destroy_term(t_terminal **t);

//signals
void load_signals(void);
void signals_handler(int signum, siginfo_t *inf, void *ctx);

//debug utils
void debug_fds(const char *message);
void debug_args(t_cmd *cmds, size_t cmds_num);
void debug_cstr_args(t_cmd *cmds, size_t cmds_num);
void debug_redirections(t_cmd *cmds, size_t cmds_num);
void catch_subprocess_segv(int n);

//organizar:
void free_cmd_args(t_cmd *cmds, int commands);

#endif /*MINISHELL_H*/
