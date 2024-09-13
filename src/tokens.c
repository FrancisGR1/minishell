#include "tokens.h"

//Tokenizes the next tokenizable cmd in the str

static bool is_var(char cmd);
static bool is_operand(char *cmd);
static void desambiguate_type(t_token *token);

t_token get_token(char *cmd)
{
	t_token tk;
//	static size_t parens;
//	static t_stack quotes;
	tk.start = NULL;
	tk.current = NULL;
	tk.type = TK_NONE;
	if (!cmd || !*cmd)
		return (tk);
	tk.start = cmd;
	tk.current = cmd;
	while (*cmd && (!is_operand(cmd) && !is_var(*cmd) /*(e não está entre "" ou '')*/))
	{
		tk.current = cmd;
		//verificar se está entre () -> var contador
		//verificar se é " ou '  -> pilha
		cmd++;
	} 
	desambiguate_type(&tk);
	ft_putns(tk.start, tk.current+1 - tk.start);
	return (tk);
}

void next_token(char **cmd, t_token tk)
{
	if (!cmd || !*cmd || !**cmd)
		return ;
	*cmd = ++tk.current;
	while (**cmd != '\0' && ft_isspace(**cmd))
		(*cmd)++;
}


static bool is_operand(char *cmd)
{
	if (!cmd || !*cmd) 
		return (false);
	return (*cmd == '|' || *cmd == '>' || *cmd == '<' || (*cmd == '&' && *(cmd + 1) == '&'));
}

static bool is_var(char cmd)
{
	if (!cmd)
		return (false);
	return (cmd == '$' || cmd == '=');
}

static void desambiguate_type(t_token *token)
{
	if (is_operand(token->start))
	{
		if (*token->start == *(token->start+1))	//operador de duplo carateres 
		{
			if (*token->start == '|')
				token->type = TK_OR_OR;
			else if (*token->start == '<')
				token->type = TK_LESS_LESS;
			else if (*token->start == '>')
				token->type = TK_MORE_MORE;
			else if (*token->start == '&')
				token->type = TK_AND_AND;
			else
				token->type = TK_ERROR;
			token->current++;
		}
		else if (token->start == token->current)//operador de único caráter 
		{
			if (*token->start == '|')
				token->type = TK_PIPE;
			else if (*token->start == '*')
				token->type = TK_STAR;
			else if (*token->start == '=')
				token->type = TK_EQUALS;
			else if (*token->start == '<')
				token->type = TK_LESS;
			else if (*token->start == '>')
				token->type = TK_MORE;
			else
				token->type = TK_ERROR;
		}
		else
			token->type = TK_ERROR;

	}
	else if (is_var(*token->start))
	{
		if (*token->start == '=')
			token->type = TK_EQUALS;
		else
		{
			token->current++;
			while (ft_isalnum(*token->current)) 	//apontar para o fim da variável 
				token->current++;
			token->type = TK_DOLLAR;
			printf("IS VAR\n");
		}
	}
	else if (*(token->start - 1) == '=' && (*(token->start) == '\"' || ft_isalnum(*(token->start))))
		token->type = TK_STR_LITERAL;
	else if (*(token->current+1) == '=')
		token->type = TK_VAR;
	else
		token->type = TK_CMD;
}

//EXEMPLOS:
//cmd operando cmd
//operando
//cmd
//var atribuição cmd 
//var
//
//Passar os espaços
//	Agora pode ser:
//		A declaração de uma variável
//		Um comando
//		Um operando
