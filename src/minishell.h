#ifndef MINISHELL_H
# define MINISHELL_H

//Readline includes
#include <readline/readline.h>
#include <readline/history.h>
#include <readline/history.h>

//Our LIB
#include "../libft/libft.h"

//Src includes
#include "tokens.h"
#include "parse.h"

//REPL cycle
void reader_loop(void);

#endif /*MINISHELL_H*/
