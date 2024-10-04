#ifndef MINISHELL_H
# define MINISHELL_H



//Readline includes
#include <readline/readline.h>
#include <readline/history.h>
#include <readline/history.h>

//wait() 
#include <sys/wait.h>
//open() 
#include <fcntl.h> 
//Our LIB
#include "../libft/libft.h"

//Pipes
#define CHILD 0

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

//Main struct
typedef struct s_terminal t_terminal;

//All the data and meta-data of a commad
typedef struct s_command t_cmd;

struct s_command
{
	t_string binary;
	t_string *args;
	t_queue *redirs;
};

struct s_terminal
{
	t_string *first_cmd;
	t_string *last_cmd;
	t_cmd *cmds;
	size_t cmds_num;
};

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

//REPL cycle
void reader_loop(void);

//parse
t_cmd *parse(t_string input, t_terminal *t);

//execution
int exec(t_cmd *cmds, t_terminal *t);

//redirection: heredoc
int heredoc(char *delimiter, int heredoc_file);
#endif /*MINISHELL_H*/
