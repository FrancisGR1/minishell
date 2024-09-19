#include "minishell.h"

t_ast  *parse(const char* const cmd)
{
	char *s;
	t_ast *tree;
	t_token *tk;
	t_terminal *t;

	tree = NULL;
	s = (char *)cmd;	//preserva o string original
	t = init_term();
	t_da *tks = da_init(sizeof(*tk));
	while (*s)   				//processa o comando
	{
		tk = get_token(s, t); 		//obtém o próximo token no string	
		printf("%s\n", tk->start);
		//guardar todos os tokens temporariamente
		//para debuggin
		if (!tk->prev)
			da_append(tks, tk);
		printf("added:\n");
		ft_putns(tk->start, tk->current+1 - tk->start);
		//insert_node(tk, &tree); 	//insere o token na árvore
		next_token(&s, tk);		//passa espaços e parênteses
	}
	size_t total = tks->cnt;
	printf("\n\n\n\n");
	printf("enter\n");
	for (size_t i = 0; i < total; i++)
	{
		printf("it: %zu\n", i);
		t_token *token = &((t_token *)(tks->data))[i];
		ft_putns(token->start, token->current + 1 - token->start);
	}
	return (tree);
}

