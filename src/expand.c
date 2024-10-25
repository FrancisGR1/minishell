#include "minishell.h"

void expand(t_string *s, char **env, int exit_code, int start)
{
	static int i;
	printf("entered expand(): %d\n", i++);
	printf("starting at: %d\n", start);
	const int dollar_pos = string_find(*s, start, s->len, "$");
	printf("ended at: %d\n", dollar_pos);
	const int dollar_end_pos = dollar_pos + str_iter(*s, dollar_pos + 1, s->len - 1 - dollar_pos, ft_isalnum);
	if (dollar_pos < 0 || (dollar_pos == 0 && s->len == 1))
		return ;
	int offset = ((s->s + dollar_end_pos) - (s->s + dollar_pos));
	if (dollar_end_pos > 0 && !offset)
		return expand(s, env, exit_code, dollar_end_pos + 1);
	if (offset <= 0)
		return str_free_and_replace_raw(s, "$");
	int size = ++offset;
	t_string delim = cstr_to_str_ptr(s->s + dollar_pos, size);
	char *delimiter = string_convert_back(delim);
	char *tmp_env = env_lookup(env, delimiter + 1);
	t_string env_str; 
	if (tmp_env)
		env_str = cstr_to_str(&tmp_env[delim.len]);
	else
		env_str =  new_str(NULL, 0);
	int len = 0;
	t_string *parts = string_divide(*s, delim, &len);
	//se só tivermos a variável no string "$var"
	if (!parts)
	{
		//str_free_and_replace_str(s, &env_str);
		if (s->type == STR_ALLOCATED)
			string_free(s);
		//se a var do env for nula "$asd"
		if (!env_str.s)
		{
			free(delimiter);
			return ;
		}
		//último caso: só temos uma variável válida: "$PWD"
		*s = str_dup(env_str);
		string_free(&env_str);
		free(delimiter);
		return ;
	}
	t_string res = new_str(NULL, 0);
	bool not_load = true;
	if (len == 1)
	{
		if (delim.s < parts[0].s)
			res = str_cat(env_str, parts[0]);
		else
			res = str_cat(parts[0], env_str);
	}
	else
	{
		for (int idx = 0, j = 0; idx < len + 1; ++idx)
		{
			if (not_load && delim.s < parts[j].s)
			{
				res = str_cat(res, env_str);
				not_load = false;
			}
			else
				res = str_cat(res, parts[j++]);
		}
	}
	free(delimiter);
	string_free(&env_str);
	free(parts);
	str_free_and_replace_str(s, &res);
	expand(s, env, exit_code, start);
}
