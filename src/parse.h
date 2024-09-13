#ifndef PARSE_H
# define PARSE_H

typedef struct TMP
{
	char *s;
} t_ast;

t_ast  *parse(const char *cmd);

#endif /*PARSE_H*/
