#include "../src/minishell.h"

static void TEST_tokenization_one_cmd(void);
static void TEST_tokenization_multi_cmd(void);
static void TEST_tokenization_multi_cmd_func(char *prompt, char **expected, int test_num);
static bool TEST_prompt(char *result, char *expected);
static bool TEST_prompt_multi(char **result, size_t size, char **expected);

#define MAX_CMD_CHAR 100
#define MAX_CMD	20 

int main(void)
{
	ft_fprintf(OUT, "\n\t\tTOKENIZATION TESTS\n");
	TEST_tokenization_one_cmd();
	TEST_tokenization_multi_cmd();
}

static char **GET_tokens(char *prompt, size_t size, char **expected)
{
	char **res = malloc(size);
	t_token tk;

	for (int i = 0; i < size;++i)
	{
		res[i] = malloc(MAX_CMD_CHAR);
		ft_memset(res[i], 0, MAX_CMD_CHAR);
		tk = get_token(prompt);
		next_token(&prompt, tk);
		size_t tk_size = tk.current + 2 - tk.start;
		ft_strlcpy(res[i], tk.start, tk.current + 2 - tk.start);
	}
	return (res);
}


static void TEST_tokenization_multi_cmd_func(char *prompt, char **expected, int test_num)
{
	char *prompt_cpy = prompt;

	int size = 0;
	for (int i = 0; expected[i]; ++i)
		size++;
	char **res = GET_tokens(prompt_cpy, size, expected);
	bool success = true;
	if (!TEST_prompt_multi(res, size, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n", test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected:\n\t%s{", RED);
		for (int i = 0; i < size; i++)
		{
			if (i + 1 < size)
				ft_fprintf(OUT, "%s, ", expected[i]);
			else 
				ft_fprintf(OUT, "%s", expected[i]);
		}
		ft_fprintf(OUT, "}%s\n", RESET);
		ft_fprintf(OUT, "\tGot:\n\t%s", RED);
		for (int i = 0; i < size; i++)
			ft_fprintf(OUT, "%s ", res[i]);
		ft_fprintf(OUT, "%s\n", RESET);
		success = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
}

static void TEST_tokenization_multi_cmd(void)
{
	int test_num = 0;

	ft_fprintf(OUT, "\nComplex commands\n-------\n");

		char *expected1[MAX_CMD] = {"ls", "|", "grep \".c\""};
		TEST_tokenization_multi_cmd_func("ls | grep \".c\"", expected1 , ++test_num);
		//--------------------------------------------------------------------
		char *expected2[MAX_CMD] = {"cd ..", "&&", "echo \"hello\" \"world\""};
		TEST_tokenization_multi_cmd_func("cd .. && echo \"hello\" \"world\"", expected2 , ++test_num);
		//--------------------------------------------------------------------
		char *expected3[MAX_CMD] = {"cat *", "|", "wc -l" };
		TEST_tokenization_multi_cmd_func(" cat * | wc -l ", expected3 , ++test_num);
		//--------------------------------------------------------------------
		char *expected4[MAX_CMD] = {"echo \'hello world\'" };
		TEST_tokenization_multi_cmd_func("echo \'hello world\'", expected4 , ++test_num);
		//TODO: qual é a melhor aboradgem para processar redireções?
		//--------------------------------------------------------------------
		char *expected6[MAX_CMD] = {"cat", "< input.txt"};
		TEST_tokenization_multi_cmd_func("cat < input.txt", expected6 , ++test_num);
		//--------------------------------------------------------------------
		char *expected8[MAX_CMD] = {"grep 'pattern' file.txt", "|", "sort"};
		TEST_tokenization_multi_cmd_func("grep 'pattern' file.txt | sort", expected8 , ++test_num);
		//--------------------------------------------------------------------
		char *expected9[MAX_CMD] = {"ls -la", ">", "out.txt"};
		TEST_tokenization_multi_cmd_func("ls -la > out.txt", expected9 , ++test_num);
		//--------------------------------------------------------------------
		char *expected10[MAX_CMD] = {"cat file1.txt file2.txt", "|", "grep 'data'"};
		TEST_tokenization_multi_cmd_func("cat file1.txt file2.txt | grep 'data'", expected10 , ++test_num);
		//--------------------------------------------------------------------
		char *expected14[MAX_CMD] = {"ls", "> file.txt", "&&", "cat file.txt"};
		TEST_tokenization_multi_cmd_func("ls > file.txt && cat file.txt", expected14 , ++test_num);
		//--------------------------------------------------------------------
		char *expected15[MAX_CMD] = {"echo hello", ">> log.txt"};
		TEST_tokenization_multi_cmd_func("echo \"hello\" >> log.txt", expected15 , ++test_num);
		//--------------------------------------------------------------------
		char *expected16[MAX_CMD] = {"cat", "<<", "END"};
		TEST_tokenization_multi_cmd_func("cat << END", expected16 , ++test_num);
		//--------------------------------------------------------------------
		char *expected17[MAX_CMD] = {"echo $?", "&&", "echo Done"};
		TEST_tokenization_multi_cmd_func("echo $? && echo Done", expected17 , ++test_num);
		//--------------------------------------------------------------------
		char *expected18[MAX_CMD] = {"mkdir test", "&&", "cd test"};
		TEST_tokenization_multi_cmd_func("mkdir test && cd test", expected18 , ++test_num);
		//--------------------------------------------------------------------
		char *expected19[MAX_CMD] = {"ls- l", "|", "grep \".c\"", "|", "wc", "-l"};
		TEST_tokenization_multi_cmd_func("ls -l | grep \".c\" | wc -l", expected19 , ++test_num);
		//--------------------------------------------------------------------
		char *expected21[MAX_CMD] = {"grep \"hello world\"", "< file.txt"};
		TEST_tokenization_multi_cmd_func("grep \"hello world\" < file.txt", expected21 , ++test_num);
		//--------------------------------------------------------------------
		char *expected22[MAX_CMD] = {"cat file.txt", "|", "grep \'pattern\'", "&&", "echo\'Found\'"};
		TEST_tokenization_multi_cmd_func("cat file.txt | grep 'pattern' && echo 'Found'", expected22 , ++test_num);
		//--------------------------------------------------------------------
		char *expected23[MAX_CMD] = {"echo \"error\"", "> error.log"};
		TEST_tokenization_multi_cmd_func("echo \"error\" > error.log", expected23 , ++test_num);
		//--------------------------------------------------------------------
		char *expected25[MAX_CMD] = {"echo hello", "&&", "echo world", "||", "echo error"};
		TEST_tokenization_multi_cmd_func("echo hello && echo world || echo error", expected25 , ++test_num);
		//--------------------------------------------------------------------
		char *expected26[MAX_CMD] = {"cd ..", "&&", "pwd"};
		TEST_tokenization_multi_cmd_func("cd .. && pwd", expected26 , ++test_num);
		//--------------------------------------------------------------------
		char *expected27[MAX_CMD] = {"echo \"Wildcard * test\"", "|", "grep \'test\'"};
		TEST_tokenization_multi_cmd_func("echo \"Wildcard * test\" | grep 'test'", expected27 , ++test_num);
		//--------------------------------------------------------------------
		char *expected28[MAX_CMD] = {"ls -l", "|", "awk \'{print $1}\'"};
		TEST_tokenization_multi_cmd_func("ls -l | awk '{print $1}'", expected28 , ++test_num);
		//--------------------------------------------------------------------
		char *expected29[MAX_CMD] = {"ps -aux", "|", "grep", "\"process\"", "|", "cut -d : -f2"};
		TEST_tokenization_multi_cmd_func("ps aux | grep \"process\" | cut -d : -f2", expected29 , ++test_num);
		//--------------------------------------------------------------------
		char *expected31[MAX_CMD] = {"rm -rf /tmp/test", "&&", "echo \"Done\""};
		TEST_tokenization_multi_cmd_func("rm -rf /tmp/test && echo \"Done\"", expected31 , ++test_num);
}


static bool TEST_prompt_multi(char **result, size_t size, char **expected)
{
	for (int i = 0; i < size; i++)
	{
		size_t n = ft_strlen(result[i]) > ft_strlen(expected[i]) ? ft_strlen(result[i]) : ft_strlen(expected[i]);
		if (ft_strncmp(result[i], expected[i], n) != 0)
			return (false);
	}
	return (true);
}

//TODO: mudar o código abaixo
//Tests only correct inputs
static void TEST_tokenization_one_cmd(void)
{
	char *original = malloc(MAX_CMD_CHAR + 1);
	char *prompt = original; 
	char *new_prompt = original; 
	char *expected = NULL;
	size_t size = 0;
	t_token result; 
	bool res = true;
	int test_num = 0;


	ft_fprintf(OUT, "\nSimple commands\n-------\n");

	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "cd ..";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "cd ..";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\t Got: %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "(cd ..)";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "cd ..";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "cd ..)";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "cd ..";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "echo hello";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "echo hello";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//.....
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "var1=$(ls)";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "var1=$(ls)";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "var1=\"hello | && echo troll\"";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "var1=\"hello | && echo troll\"";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "|";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "|";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "||";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "||";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "&&";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "&&";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "<";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "<";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = "<<";	
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = "<<";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = ">";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = ">";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//-----------------------------------------------------------------
	test_num++;
	prompt = original; 
	new_prompt = ">>   ";
	size = ft_strlen(new_prompt);
	strncpy(prompt, new_prompt,size);
	prompt[size] = '\0';
	expected = ">>";
	result = get_token(prompt);
	*(++result.current) = '\0';
	if (!TEST_prompt(result.start, expected))
	{
		ft_fprintf(OUT, "%d: %s[KO]%s\n",test_num, RED, RESET);
		ft_fprintf(OUT, "\tExpected: %s\n\tGot:      %s\n", expected, result.start);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", test_num, GRN, RESET);
	//
	//.......-.-.-.-.--.......-.-.-.-.--.......-.-.-.-.--.......-.-.-.-.-
	//Outros testes aqui
	if (!res)
		ft_fprintf(OUT, "\n%sFAILED%s\n\n\n", BRED, RESET);
	else
		ft_fprintf(OUT, "\n%sSUCCESS%s\n\n\n", BGRN, RESET);
	free(original);
}


static bool TEST_prompt(char *result, char *expected)
{
	if (!result || !expected)
	{
		ft_fprintf(OUT, "SOMETHING IS NULL\n");
		return (false);
	}
	size_t size = ft_strlen(result) > ft_strlen(expected) ? ft_strlen(result) : ft_strlen(expected);
	return (ft_strncmp(result, expected, size) == 0);
}
