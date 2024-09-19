#ifndef MINISHELL_H
# define MINISHELL_H



//Readline includes
#include <readline/readline.h>
#include <readline/history.h>
#include <readline/history.h>

//Our LIB
#include "../libft/libft.h"


//Estrutura principal
typedef struct s_struct t_terminal;

//Tokens
typedef struct s_token t_token;

//Src includes
#include "tokens.h"
#include "parse.h"

struct s_struct
{
	t_token *last;
};

//REPL cycle
void reader_loop(void);

//terminal
t_terminal *init_term(void);

#endif /*MINISHELL_H*/
