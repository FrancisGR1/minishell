#ifndef MINISHELL_H
# define MINISHELL_H

//Our LIB
#include "../libft/libft.h"
#include "tokens.h"
#include "parse.h"

//Readline includes
#include <readline/readline.h>
#include <readline/history.h>
#include <readline/history.h>


//REPL cycle
void reader_loop(void);

#endif /*MINISHELL_H*/
