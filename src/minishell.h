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
	t_string *args;
};

struct s_terminal
{
	char *last;
};

//REPL cycle
void reader_loop(void);

//terminal
t_terminal *init_term(void);

//parse
t_string *parse(t_string cmd);

//exec
int *exec(t_string *cmd);
#endif /*MINISHELL_H*/
