#include "minishell.h"

t_terminal *init_term(void)
{
	t_terminal *t;

	t = malloc(sizeof(t_terminal));
	return(t);
}
