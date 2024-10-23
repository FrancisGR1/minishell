#include "minishell.h"

void expand(t_string *s, char **env, int exit_code)
{
	const int dollar_pos = string_find(*s, 0, s->len, "$");
	int dollar_end_pos = str_iter(*s, dollar_pos + 1, s->len - 1 - dollar_pos, ft_isalnum);
	t_string before_dollar;
	t_string dollar;
	t_string after_dollar;
	t_string env_str;

	if (dollar_pos < 0)
		return ;
	before_dollar = cstr_to_str_ptr(s->s, dollar_pos);
	dollar = cstr_to_str_nsize(s->s + dollar_pos, dollar_end_pos + 1 - dollar_pos);
	str_advance_ptr(&dollar);
	if (ft_strncmp(dollar.s, "?", dollar.len) == 0)
	{
		printf("comparing\n");
		char *num = ft_itoa(exit_code);
		env_str = cstr_to_str(num);
		free(num);
		dollar_end_pos += 1;
	}
	else 
	{
		env_str = cstr_to_str(env_lookup(env, dollar.s));
		int equal_pos = str_iter(env_str, 0, env_str.len, ft_isalnum);
		str_advance_ptr_ntimes(&env_str, equal_pos + 1);
	}
	if (!env_str.s)
		str_free_and_replace_raw(&env_str, "");
	after_dollar = cstr_to_str_ptr(s->s + dollar_end_pos, s->len - dollar_end_pos);
	int total_size = before_dollar.len + env_str.len + after_dollar.len;
	t_string result = str_join(3, total_size, before_dollar, env_str, after_dollar);
	ft_fprintf(STDOUT, "result: %S\n", result);
	str_free_and_replace_str(s, &result);
	expand(s, env, exit_code);
}
