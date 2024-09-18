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
	TK_CMD_REDIR,			// <cmd> <args> <redir>
	TK_STR_LITERAL,		//"<texto>"
	TK_ASSIGNMENT,		// <var>=<value>
	//expansíveis
	TK_DOLLAR,		// $
	TK_STAR, 		// *
	
	//atribuição
	//Redireções
	TK_LESS,		// <
	TK_MORE,		// >
	TK_LESS_LESS,		// <<
	TK_MORE_MORE,		// >>
				//
	//operandos
	TK_AND_AND,		// &&
	TK_OR_OR,		// ||
	TK_PIPE,		// |
	
	TOKENS
}	tk_type;

typedef struct s_token
{
	char *start;
	char *current;
	int parens;
	tk_type type;
	//Se o comando conter redireções, temos uma lista de ligação dupla
		//exemplo: echo "hello" > out.file < in.file
	//caso contrário os tokens são nulos
	struct s_token *next; //lista de tokens associados
	struct s_token *prev;
}	t_token;

t_token *get_token(char *cmd, t_terminal *t);
void next_token(char **cmd, t_token *tk);

#endif /*TOKENS_H*/
