#include "../src/minishell.h"

static void TEST_tokenization(void);
static bool test_prompt(char *prompt, char *expected);

int main(void)
{
	TEST_tokenization();
}

static void TEST_tokenization(void)
{
	char *prompt = NULL;
	bool res = true;
	int i = 0;

	ft_fprintf(OUT, "\n\t\tTOKENIZATION TESTS\n");

	ft_fprintf(OUT, "\nSimple commands\n-------\n");
	prompt = "cd ..";
	++i;
	if (!test_prompt("cd ..", "cd .."))
	{
		ft_fprintf(OUT, "%d: %sFAILED%s\n",i, RED, RESET);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", i, GRN, RESET);
	++i;
	if (!test_prompt("cd ..", "cd .."))
	{
		ft_fprintf(OUT, "%d: %sFAILED%s\n",i, RED, RESET);
		res = false;
	}
	else
		ft_fprintf(OUT, "%d: %s[OK]%s\n", i, GRN, RESET);
	//Outros testes aqui
	if (!res)
		ft_fprintf(OUT, "\n%sFAILED%s\n\n\n", BRED, RESET);
	else
		ft_fprintf(OUT, "\n%sSUCCESS%s\n\n\n", BGRN, RESET);
}


static bool test_prompt(char *prompt, char *expected)
{
	size_t size = ft_strlen(prompt) > ft_strlen(expected) ? ft_strlen(prompt) : ft_strlen(expected);
	return (ft_strncmp(prompt, expected, size) == 0);
}
