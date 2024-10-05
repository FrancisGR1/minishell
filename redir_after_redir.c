#include "src/minishell.h"

void debug_fds(const char *message) {
    char proc_path[64];
    char target[1024];
    printf("\n=== %s ===\n", message);
    
    // Print info for FDs 0-9 (adjust range as needed)
    for (int fd = 0; fd < 10; fd++) {
        snprintf(proc_path, sizeof(proc_path), "/proc/self/fd/%d", fd);
        ssize_t len = readlink(proc_path, target, sizeof(target) - 1);
        if (len != -1) {
            target[len] = '\0';
            printf("FD %d -> %s\n", fd, target);
        }
    }
    printf("\n");
}

int main(void)
{
	char *args[] = {"cat", "-e", NULL};
	int input_fd = open("tmp", O_RDONLY);
	int orig = dup(STDIN);
	fprintf(stdout, "before dup2:\n\tinput_fd: %d\n\tSTDIN: %d\n\torig:%d\n", input_fd, STDIN, orig);
	debug_fds("BEFORE");
	dup2(input_fd, STDIN);
	fprintf(stdout, "after dup2:\n\tinput_fd: %d\n\tSTDIN: %d\n\torig:%d\n", input_fd, STDIN, orig);
	debug_fds("AFTER");
	int id = fork();
	if (id == 0)
		execvp(args[0],args);
	while (wait(NULL) > 0)
		;
	dup2(orig, STDIN);
	close(orig);
	close(input_fd);
	id = fork();
	if (id == 0)
		execvp(args[0],args);
	while (wait(NULL) > 0)
		;
}
