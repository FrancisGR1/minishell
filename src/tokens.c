#include "tokens.h"

//Tokenizes the next tokenizable cmd in the str

static bool is_operand(char *cmd);
static bool is_quote(char c);
static void desambiguate_type(t_token *token);

t_token get_token(char *cmd)
{
	t_token tk;
	bool inside_quotes;
	char quote;
	static int parens;
	tk.start = NULL;
	tk.current = NULL;
	tk.type = TK_NONE;
	inside_quotes = false;
	quote = '\0';
	if (!cmd || !*cmd)
		return (tk);
	tk.start = cmd;
	tk.current = cmd;
	while (*cmd && (!is_operand(cmd) || inside_quotes || *cmd == ')'))
	{
		tk.current = cmd;
		if (*cmd == '=') 
			break ;
		if (*cmd == '(' && !inside_quotes)
			parens++;
		if (is_quote(*cmd) && (*cmd == quote || !quote))
		{
			if (*cmd == quote)
				inside_quotes = false;
			else
				inside_quotes = true;
			quote = *cmd;
		}
		cmd++;
	} 
	tk.parens = parens;
	while (*cmd++ == ')') //passar as parênteses
		parens--;
	desambiguate_type(&tk);
	//ft_putns(tk.start, tk.current+1 - tk.start);
	//printf("\t\tnested level: %d\n", tk.parens);

	//CLEANUP TOKEN
	while ((*tk.start == '(' || *tk.start == ' ') && tk.start)
		tk.start++;
	while ((*tk.current == ')' || *tk.current == ' ') && tk.current)
		tk.current--;
	return (tk);
}

void next_token(char **cmd, t_token tk)
{
	if (!cmd || !*cmd || !**cmd)
		return ;
	*cmd = ++tk.current;
	while (**cmd != '\0' && (ft_isspace(**cmd) || **cmd == ')'))
		(*cmd)++;
}


static bool is_operand(char *cmd)
{
	if (!cmd || !*cmd) 
		return (false);
	return (*cmd == '|' || *cmd == '>' || *cmd == '<' || (*cmd == '&' && *(cmd + 1) == '&') || *cmd == '$');
}

static bool is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

static void desambiguate_type(t_token *token)
{
	if (!token->start || !token->current)
		return ;
	if (is_operand(token->start))
	{
		if ((token->start + 1) != NULL && *token->start == *(token->start+1)) // 2 chars
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
		else if (token->start == token->current) //1 char
		{
			if (*token->start == '|')
				token->type = TK_PIPE;
			else if (*token->start == '*')
				token->type = TK_STAR;
			else if (*token->start == '<')
				token->type = TK_LESS;
			else if (*token->start == '>')
				token->type = TK_MORE;
			else if (*token->start == '$')
			{
				token->type = TK_DOLLAR;
				while (*token->current && ft_isalnum(*(++token->current))) //fim da var
					;
			}
			else
				token->type = TK_ERROR;
		}
		else //Teoricamente nunca devíamos chegar aqui
			token->type = TK_ERROR;

	}
	else if (*token->current == '=') //temos uma variável
	{
		token->type = TK_ASSIGNMENT;
		if ((token->current-1) == NULL || !ft_isalnum(*(token->current-1))) //char antes
			token->type = TK_ERROR;
		else if ((token->current+1) == NULL || (!ft_isalnum(*(token->current+1)) && !is_quote(*(token->current+1)) &&  *(token->current+1) != '$')) //char depois
			token->type = TK_ERROR;
		else
		{
			char quote = '\0';
			token->current++;
			if (*token->current == '\"' || *token->current =='\'') //se o rvalue começar com aspas
			{
			quote = *token->current;
				token->current++;
				while (*token->current && *token->current != quote) 
					token->current++;
			}
			else							
			{
				if (*token->current == '$')
					token->current++;
				while (token->current && (ft_isalnum(*(token->current)) || *(token->current) == '(' || *(token->current) == ')')) //fim do rvalue; AFAZER: verificar nº de parens
					token->current++;
			}
		}

	}
	else
		token->type = TK_CMD;
}

