/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/20 00:03:16 by frmiguel          #+#    #+#             */
/*   Updated: 2024/10/20 00:03:16 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// our lib
# include "../libft/libft.h"

//TODO: TEMPORÁRIO
#include <assert.h>

// Readline includes
# include <readline/history.h>
# include <readline/readline.h>

// wait()
# include <sys/wait.h>
// open()
# include <fcntl.h>
// stat(), S_ISREG()
# include <sys/stat.h>

// error handling
# include <errno.h>

// global variable with last value received
extern int						g_sig_received;

// special characters
# define PIPE '\1'
# define LESS '\2'
# define MORE '\3'
# undef SPACE
# define SPACE '\4'
# define DELIMITERS "\1\2\3\4"

// Prompts
# define RL_PROMPT BGRN "minishell> " RESET
# define HEREDOC_PROMPT "> "

// Max cmds possible in minishell
# define CMD_MAX 1000

// Exit codes
# define NO_ERROR 0
# define NO_INPUT 0
# define FILE_ERROR 1
# define WRONG_FORMAT 2
# define NOT_EXECUTABLE 126 
# define CMD_NOT_FOUND 127
# define FATAL_ERROR 128

// Pipe ends
# define PIPE_WRITE 1
# define PIPE_READ 0

// Pid of subprocess spawned is 0
# define SUBPROCESS 0

//output redirections open files in this mode
# define DEFAULT_FILE_PERM 0644

//main parent ingnores every signal
# define DO_NOTHING -1
# define DEFAULT 0

// Main struct
typedef struct s_terminal		t_terminal;

// All the data and meta-data of a commad
typedef struct s_command		t_cmd;

// redirections
typedef struct s_redirections	t_redir;

// Stores the temporary data for/from parsing and
// the important data for convenience
typedef struct s_parser_buffer
{
	t_string					*pipe_sides;
	t_string					*args_ptr;
	t_dynamic_array				*redir_ptrs;
	int							split_len;
	int							idx;
	int							redir_idx;
	t_terminal					*t;
	t_cmd						*cmds;
	t_string					*redir_ptr;
}								t_parser_buffer;

enum							e_redir_type
{
	REDIR_INPUT,
	REDIR_OUTPUT,
	REDIR_APPEND,
	REDIR_HEREDOC,
};

struct							s_redirections
{
	enum e_redir_type			type;
	t_string					fd;
	int							location;
};

struct							s_command
{
	t_string					binary;
	t_string					*args;
	size_t		argc;
	char						**cstr_args;
	t_list						*redirs;
	//redir info
	t_redir						*last_input_ptr;
	t_redir						*last_output_ptr;
	bool						has_heredoc;
	char						heredoc_file[PATH_MAX];
	int						heredoc_wstatus;
};

struct							s_terminal
{
	t_cmd						*cmds;
	char						**env;
	size_t						cmds_num;
	int							terminal_fd;
	int							exit_code;
	t_string					input;
};

// parse
t_cmd							*parse(t_string input, t_terminal *t);

// parse redirections
t_redir							*new_redir(t_string *args, t_string r_ptr);
void							remove_redirections(t_parser_buffer *pb,
		t_cmd *cmds);
bool							get_redir(t_parser_buffer *pb, t_cmd *cmds,
		int *redir_idx);
void	define_redir_type(t_redir *redir, t_string r_ptr);

//expansion
void expand(t_string *s, char **env, int exit_code, int start);

//paths
char *find_path(t_string cmd, char **env);

// execution
int								exec(t_cmd *cmds, t_terminal *t);

// redirections
int	set_redirs(t_list *redirs, char *heredoc_file, int terminal_fd, t_redir *li_ptr, t_redir *lo_ptr);
int	print_redir_error(int redir_error, t_redir *r, char *heredoc_file, char *file_name);
int	open_and_redirect_output(char *file, int flags, int terminal_fd, bool is_last_output);
int	open_and_redirect_input(char *file, int terminal_fd, bool is_last_output);

// redirection: heredoc
int	heredoc(char *delimiter, char *heredoc_file, int terminal_fd, bool is_last_input);
void wait_heredoc(int *hd_exit_status, pid_t pid);
void							write_path(char dest[], char *src);

// terminal struct utils
t_terminal						*init_term(char **env);
void							reset_term(t_terminal **t);
void							destroy_term(t_terminal **t);

// signals
void	load_signals(int at);
void							signals_handler(int signum, siginfo_t *inf,
		void *ctx);
//environment
char **env_dup(char **env);
char *env_lookup(char **env, char *target);

// errors and memory management
void							*free_on_error(int exit_code,
		char *error_message, t_parser_buffer *pb);
void							freexit(int exit_code, t_cmd *cmds,
		t_terminal *t);
void							free_cmd_cstr_args(t_cmd *cmds);
void							alloc_args(t_cmd *cmds, int commands_num);

// initializers
void							init_redirs(t_parser_buffer *pb, size_t idx);
void							init_parser(t_parser_buffer *pb, t_terminal *t);
void							init_pipes(int fds[][2], int cmds_num);

// debug utils
void							debug_fds(const char *message, int fd);
void							debug_args(t_cmd *cmds, size_t cmds_num);
void							debug_cstr_args(t_cmd *cmds, size_t cmds_num);
void							debug_redirections(t_cmd *cmds,
		size_t cmds_num);
void							catch_subprocess_segv(int n);

#endif /*MINISHELL_H*/
