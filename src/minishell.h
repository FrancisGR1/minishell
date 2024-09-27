#ifndef MINISHELL_H
# define MINISHELL_H



//Readline includes
#include <readline/readline.h>
#include <readline/history.h>
#include <readline/history.h>

//wait() 
#include <sys/wait.h>

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

//Estrutura principal
typedef struct s_terminal t_terminal;

//Toda os dados e meta-dados de um comando
typedef struct s_command t_cmd;

struct s_command
{
	t_string binary;
	t_string *args;
};

struct s_terminal
{
	char *last;
	t_cmd *cmds;
	size_t cmds_num;
};

//REPL cycle
void reader_loop(void);

//parse
t_cmd *parse(t_string input, t_terminal *t);

//exec
//int exec(t_cmd *cmds, t_terminal *t);
int exec(t_cmd *cmds, t_terminal *t);
#endif /*MINISHELL_H*/
