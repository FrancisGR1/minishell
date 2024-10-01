#include <string.h>
#include <stdio.h>


size_t array_len(void **arr)
{
	size_t len = 0;
	while (arr[len] != NULL)
		len++;
	return (len);
}

int main(void)
{
	char ls[] = "ls -al";
	char cat[] = "cat -e";
	char echo[] = "echo -n hello";
	char ping[] = "ping -c";
	char *list[] = {ls, cat, echo, ping, NULL};
	size_t arr_len = array_len((void *)list);
	printf("before: %zu\n", arr_len);
	arr_len--;
	for (int i = 0; list[i]; i++)
		printf("\t%s\n", list[i]);
	memmove(&list[1], &list[2], sizeof(char *) * 2);
	list[arr_len--] = NULL;
	printf("after: %zu\n", array_len((void *)list));
	for (int i = 0; list[i]; i++)
		printf("\t%s\n", list[i]);
}
