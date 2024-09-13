#ifndef TOKENS_H
# define TOKENS_H

#include "minishell.h"

//AFAZER: verificar precisão da precedência
typedef enum e_tk_type
{
	TK_ERROR = -1,
	TK_NONE,

	//comandos
	TK_CMD,			// <cmd> <args>
	TK_STR_LITERAL,		//"<texto>"
	TK_VAR,			// <var>
	//expansíveis
	TK_DOLLAR,		// $
	TK_STAR, 		// *
	//atribuição
	TK_EQUALS,		// =
	//operandos
	TK_AND_AND,		// &&
	TK_OR_OR,		// ||
	TK_LESS,		// <
	TK_MORE,		// >
	TK_LESS_LESS,		// <<
	TK_MORE_MORE,		// >>
	TK_PIPE,		// |
	
	TOKENS
}	tk_type;

typedef struct s_token
{
	char *start;
	char *current;
	tk_type type;
}	t_token;

t_token get_token(char *cmd);
void next_token(char **cmd, t_token tk);

#endif /*TOKENS_H*/
