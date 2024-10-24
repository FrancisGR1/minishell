#include "minishell.h"

void expand(t_string *s, char **env, int exit_code, int start)
{
	const int dollar_pos = string_find(*s, start, s->len, "$");
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
		env_str =  cstr_to_str("");
	int len = 0;
	t_string *parts = string_divide(*s, delimiter, &len);
	if (!parts)
	{
		str_free_and_replace_str(s, &env_str);
		expand(s, env, exit_code, start);
		return ;
	}
	t_string res = new_str(NULL);
	bool not_load = true;
	if (len == 1)
	{
		if (env_str.s < parts[0].s)
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
	ft_fprintf(STDOUT, "res: %S\n", res);
	str_free_and_replace_str(s, &res);
	expand(s, env, exit_code, start);
}
