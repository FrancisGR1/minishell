#include "parse.h"
#include "minishell.h"

t_ast  *parse(const char *cmd)
{
	char *s;
	t_ast *tree;
	t_token tk;

	tree = NULL;
	s = ft_strndup(cmd, ft_strlen(cmd));	//preserva o string original
	while (*s)   				//processa o comando
	{
		tk = get_token(s); 		//obtém o próximo token no string
		//insert_node(tk, &tree); 	//insere o token na árvore
		next_token(&s, tk);
	}
	return (tree);
}


