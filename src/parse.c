#include "minishell.h"

t_ast  *parse(const char* const cmd)
{
	char *s;
	t_ast *tree;
	t_token *tk;
	t_terminal *t = malloc(sizeof(t_terminal));

	tree = NULL;
	s = (char *)cmd;	//preserva o string original
	while (*s)   				//processa o comando
	{
		tk = get_token(s, t); 		//obtém o próximo token no string	
		//insert_node(tk, &tree); 	//insere o token na árvore
		next_token(&s, tk);		//passa espaços e parênteses
	}
	return (tree);
}


