#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "colors.h"
# include <sys/stat.h> // access and macros
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include "tokenizer.h"
# include "pipeline.h"
# include <errno.h>

typedef struct s_shell
{
    int     last_status;   // for $?
    char  **env;           // environment variables (array)
    // or: t_env *env_list; if we manage as a linked list
    t_input	input;
    char 	**argv;          // arguments for current command
    char   *cwd;
	t_pipeline pipeline;
}   t_shell;

typedef struct s_expand_state
{
	int	i;
	int	in_single;
	int	in_double;
	int	quote_handled;
	int	expanded;
}	t_expand_state;

//TODO move signals.h
extern volatile sig_atomic_t g_signal;
void	setup_signal_handlers_for_prompt();
void	setup_signal_handlers_for_child();
void	handle_sig(int signum);

//TODO move to executor.h
char	*join_cmd_to_path(const char *path, const char *cmd);
char	**find_from_path(char *envp[]);
char	**build_absolute_paths(char **paths, const char *cmd);
int	    has_slash(char *input); // helper in exec
int	exec_pipeline(char ***argv_per_cmd, int cmd_count, char **envp, t_pipeline *pipeline);
void	close_parent_unused_ends(int stage_index, int cmd_count,
	int (*pipe_pairs)[2]);
void	compute_cmd_fds(int cmd_index, int cmd_count, int (*pipe_pairs)[2],
	int *in_fd, int *out_fd);
void	close_all_pipes(int (*pipe_pairs)[2], int cmd_count);
int	(*allocate_pipes(int cmd_count))[2];
pid_t	spawn_cmd(char **argv, char **envp, int in_fd, int out_fd);
int	wait_for_pid_once(pid_t target_pid, int *out_raw_status);
int	wait_all_and_last_status(pid_t *child_pids, int child_count,
	pid_t last_child_pid);

//TODO move to utils.h
void	free_split(char **arr);
void	free_partial(char **arr, size_t n);

// quote_check.c
int check_quote_balance(char **line);
int	quotes_unbalanced(const char *string);
int	append_new_input(char **line, char *new_input);

// quote_removal.c
int		remove_quotes(t_input *input);
char	*handle_quote_removal(char *text, int *was_quoted);
int		create_unquoted_text(char *text, char **unquoted_text, int *was_quoted);
int		remove_outer_quote(char c, int *in_single, int *in_double);

// 01_expansion.c
int	expand_tokens(t_input *input, int last_status, char **envp);

// 02_expand_status.c
char	*expand_status(char *text, int last_status);
int		create_exp_status_text(char *text, char **exp_text, char *status_str);

// 03_expand_variable.c
char	*expand_variable(char *text, char **envp);
int		create_exp_var_text(char *text, char **exp_text, char **envp);
int		handle_var_expansion(char *text, char **exp_text, int *i, char **envp, int in_single);
int		process_var_expansion(char *text, char **exp_text, int *i, char **envp);

// 04_expand_utils.c
int	valid_cont_char(char c);
int	valid_start_char(char c);
int	process_expanded_str(char **exp_text, const char *str);
int	process_quote_char(char c, int *in_single, int *in_double, char **exp_text);
int	process_char(char **exp_text, char c);

// 05_expand_utils_2.c
void	init_expand_state(t_expand_state *st);
size_t	copy_n_chars(char *dst, const char *src, size_t size);
int		extract_key(char *text, int *i, char **key, int *key_len, int *start_i);
int		find_env_index(char **envp, char *key, int key_len);

#endif