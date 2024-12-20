/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frmiguel <frmiguel@student.42Lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/13 00:21:59 by frmiguel          #+#    #+#             */
/*   Updated: 2024/11/14 12:20:14 by frmiguel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// our lib
# include "../libft/libft.h"

// Readline includes
# include <readline/history.h>
# include <readline/readline.h>

// wait()
# include <sys/wait.h>
// open()
# include <fcntl.h>
// stat()
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

# define EMPTY_STR "\5"
# define EMPTY_EXPANDED_STR "\6"

# define HEREDOC_PROMPT "> "

// Max cmds possible in minishell
# define CMD_MAX 1000

// Parse error messages
# define ERROR_QUOTES "Format error: Quotes unclosed"
# define ERROR_NO_CMD "Format error: Missing command"
# define ERROR_NO_REDIR "Format error: No redirection file"

// Exit codes
# define NO_ERROR 0
# define NO_INPUT 0
# define FILE_ERROR 1
# define GENERAL_ERROR 1
# define WRONG_FORMAT 2
# define BUILTIN_ERROR 2
# define DONT_EXIT 3
# define NOT_EXECUTABLE 126
# define CMD_NOT_FOUND 127
# define FATAL_ERROR 128

// Pipe ends
# define PIPE_WRITE 1
# define PIPE_READ 0

// Pid of subprocess spawned is 0
# define SUBPROCESS 0

// output redirections open files in this mode
# define DEFAULT_FILE_PERM 0644

// signal modes
// ignore every signal
# define DO_NOTHING 1
/*handle ctr + c; ctrl + \ */
# define DEFAULT 2
// setup sigquit for block mode
# define BLOCK 4
# define HEREDOC 8

// array size for the storage of variables
# define ENV_INITIAL_SIZE 300

// modes of environmente variables lookup
# define VALUE 0
# define START 1

// Main struct
typedef struct s_terminal		t_terminal;

// All the data and meta-data of a commad
typedef struct s_command		t_cmd;

// redirections
typedef struct s_redirections	t_redir;

// Stores the temporary data for/from parsing
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

// every redirection has a: 1)type; 2)file number associated
struct							s_redirections
{
	enum e_redir_type			type;
	t_string					fd;
};

// general information about the redirections of a command
typedef struct s_redir_info
{
	t_redir						*li_ptr;
	t_redir						*lo_ptr;
	bool						has_heredoc;
	char						heredoc_file[PATH_MAX];
	int							heredoc_wstatus;
}								t_redir_info;

struct							s_command
{
	t_string					binary;
	t_string					*args;
	size_t						argc;
	char						**cstr_args;
	size_t						cstr_argc;
	t_list						*redirs;
	t_redir_info				ri;
	bool						only_redirs;
};

struct							s_terminal
{
	t_cmd						*cmds;
	t_list						*all_args_ptrs;
	char						**env;
	int							env_size;
	int							env_capacity;
	size_t						cmds_num;
	int							terminal_fd;
	int							exit_code;
	t_string					input;
	char						*prompt;
	bool						is_running;
	struct stat					stat;
};

// parse
t_cmd							*parse(t_string input, t_terminal *t);

// parse utils
t_string						*get_args_ptrs(t_parser_buffer *pb);
bool							format_args(t_parser_buffer *pb,
									t_cmd *current_cmd, int *redir_idx);
bool							set_cmd(t_cmd *cmds, size_t idx,
									t_string *args_ptr, t_terminal *t);
int								mark_special_characters(t_string input,
									size_t *cmds_num);
int								remove_quotes(t_string *arg);

// parse redirections
t_redir							*new_redir(t_string *args, t_string r_ptr);
void							remove_redirections(t_parser_buffer *pb,
									t_cmd *current_cmd);
bool							get_redir(t_parser_buffer *pb,
									t_cmd *current_cmd, int *redir_idx);
bool							get_redir(t_parser_buffer *pb,
									t_cmd *current_cmd, int *redir_idx);

// expansion
void							expand(t_string *s, char **env, int exit_code,
									int start);

// expansion utils
int								valid_dollar_char(int c);
void							remove_empty_codes(t_string *str);

// reformat args after expansion
void							rearrange_args_after_expansion(t_string **arg,
									int current, size_t *argc);
t_string						*make_rearranged_args(t_string *old,
									t_string *split, int cur, int c);
void							cleanup_arg(char *str);
void							alloc_args(t_cmd *cmds, int commands_num,
									char **t_env);

// paths
char							*find_path(char *cmd, char **env);
void							get_binary_path(char **cmd_args, char **t_env);

// execution
int								exec(t_cmd *cmds, t_terminal *t);

// execution helper functions
void							dup2_pipe(int fds[][2], int idx, int last);
void							close_fds(int fds[][2], int cmds_num);
bool							is_builtin(char *command);
int								exec_builtin(char **argv, int argc,
									t_terminal *t);
bool							should_exec(t_cmd *cmds, t_terminal *t);
bool							should_exec_in_main(char **argv, t_terminal *t);
bool							is_nested_term(t_cmd cmd, t_terminal *t);

// builtins
int								builtin_pwd(void);
int								builtin_unset(char **argv, int argc,
									t_terminal *t);
int								builtin_exit(char **argv, int argc,
									t_terminal *t);
int								builtin_echo(char **argv, int argc);
int								builtin_env(t_terminal *t);
int								builtin_cd(char **argv, int argc,
									t_terminal *t);
int								builtin_export(char **argv, int argc,
									t_terminal *t);

// wait for subprocesses
int								wait_subprocesses(pid_t *subprocesses,
									int commands, t_cmd *cmds);
void							wait_heredoc(int *hd_exit_status, pid_t pid);

// redirections
int								set_redirs(t_list *redirs, int terminal_fd,
									t_redir_info ri);
int								print_redir_error(int redir_error, t_redir *r,
									char *heredoc_file, char *file_name);
int								open_and_redirect_output(char *file, int flags,
									int terminal_fd, bool is_last_output);
int								open_and_redirect_input(char *file,
									int terminal_fd, bool is_last_output);

// redirection: heredoc
int								heredoc(char *delimiter, char *heredoc_file,
									int terminal_fd, bool is_last_input);
void							wait_heredoc(int *hd_exit_status, pid_t pid);
void							write_heredoc_path(char dest[], char *src);

// terminal struct utils
t_terminal						*init_term(char *program, char **env);
void							reset_term(t_terminal **t);
int								destroy_term(t_terminal **t);
bool							term_should_stop_running(t_terminal *t);

// signals
void							load_signals(int at);
void							signals_handler(int signum, siginfo_t *inf,
									void *ctx);
void							heredoc_handler(int signum, siginfo_t *inf,
									void *ctx);
void							load_subprocess_signals(t_cmd *current_cmd,
									t_terminal *t);
// environment
char							**env_dup(char **env, t_terminal *t);
char							*env_lookup(char **env, char *key,
									int should_lookup);
void							env_set(char **env, char *key, char *new_value,
									t_terminal *t);
int								env_get_idx(char **env, char *key);

// errors and memory management
void							*frerror(int exit_code, char *error_message,
									t_parser_buffer *pb);
void							freexit(int exit_code, t_terminal *t);
void							free_cmd_args(t_cmd *current_cmd);

// wrappers
void							ft_add_history(t_string input);
t_string						ft_readline(char *prompt, t_terminal *t);
void							safe_dup2(int fd, int duplicate_to);
void							safe_close(int fd_to_close);

// initializers
void							init_cmds(t_parser_buffer *pb, size_t idx);
void							init_parser(t_parser_buffer *pb, t_terminal *t);
void							init_pipes(int fds[][2], int cmds_num);

// debug utils
/*
void							debug_fds(const char *message, int fd);
int	debug_cmds(char *msg, t_cmd *cmds,
									size_t cmds_num);
int	debug_cstr_args(char *msg, t_cmd *cmds,
									int cmds_num);
int								debug_args(char *msg, t_string *args);
int	debug_redirections(t_cmd *cmds,
									size_t cmds_num);
void							catch_subprocess_segv(int n);
*/
#endif /*MINISHELL_H*/
