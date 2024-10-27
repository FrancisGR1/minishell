#include "minishell.h"

static int is_special_char(int c);
void expand(t_string *s, char **env, int exit_code, int start)
{
	static int i;
	printf("entered:\n");
	if (!s)
		return;
	const int dollar_pos = string_find(*s, start, s->len, "$");
	const int dollar_end_pos = dollar_pos + str_iter(*s, dollar_pos + 1, s->len - 1 - dollar_pos, ft_isalnum);
	if (dollar_pos < 0)
	{
		printf("%d: dollar_pos < 0\n", i++);
		return ;
	}
	int offset = ((s->s + dollar_end_pos) - (s->s + dollar_pos));
	//se só tivermos um dólar
	if (s->s + dollar_end_pos + 1)
		if (is_special_char(*(s->s + dollar_end_pos + 1)))
			offset++;
	if (dollar_pos >= 0 && !offset)
	{
		return expand(s, env, exit_code, dollar_pos + 1);
	}
	int size = ++offset;
	t_string delim = cstr_to_str_ptr(s->s + dollar_pos, size);
	ft_fprintf(STDOUT, "DELIMITER: %S\n", delim);
	char *delimiter = string_convert_back(delim);
	t_string env_str;
	//apanho aqui
	if (ft_strncmp(delimiter, "$?", 2) == 0)
	{
		char *num = ft_itoa(exit_code);
		env_str = cstr_to_str(num);
		free(num);
	}
	else {
		char *tmp_env_ptr = env_lookup(env, delimiter + 1);
		if (tmp_env_ptr)
			env_str = cstr_to_str(tmp_env_ptr);
		else
			env_str = cstr_to_str("");
	}
	int len = 0;
	//as partes são pointers a apontar para o início/fim das secções 
	//que NÂO fazem parte da variável $
	t_string *parts = string_divide(*s, delim, &len);
	if (!parts)//se só tivermos a variável no string "$var" 
	{
		//porque é que isto está a dar leaks de 1 byte?
		string_free(s);
		*s = str_dup(env_str);
		string_free(&env_str);
		free(delimiter);
		return ;
	}
	t_string res = new_str(NULL, 0);
	//bool not_load = true;
	t_string res1;
	t_string res2;
	t_string res3;
	//se só tivermos 2 partes (a var e a parte à esquerda/direita)
	if (len == 1)
	{
		if (delim.s < parts[0].s)
			res = str_cat(env_str, parts[0]);
		else
			res = str_cat(parts[0], env_str);
	}
	//caso contrário temos 3 partes (a variável é o meio)
	else 
	{
		//tenho de duplicar visto que parts são pointers
		res1 = str_dup(parts[0]);
		res2 = str_cat(res1, env_str);
		string_free(&res1);
		res3 = str_cat(res2, parts[1]);
		string_free(&res2);
		res = res3;

	}
	ft_fprintf(STDOUT, "RESULT: %S\n", res);
	free(delimiter);
	string_free(&env_str);
	free(parts);
	if (s->type == STR_ALLOCATED)
		string_free(s);
	*s = str_dup(res);
	string_free(&res);
	expand(s, env, exit_code, start);
}

static int is_special_char(int c)
{
	return (c == '?' || c == '!' || c == '#' || 
			c == '&' || c == '$' || c == '*' || 
			c == '@' || c== '_' || ft_isdigit(c));
}
