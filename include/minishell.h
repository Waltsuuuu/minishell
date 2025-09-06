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
# include <errno.h>


/* ---------- Command (one pipeline segment) ---------- */

typedef struct s_cmd
{
	char	**argv;     /* NULL-terminated, exec-ready */
	int		is_builtin; /* 0/1 flag (fill after parse) */
}	t_cmd;

/* ---------- Environment as linked list ---------- */

/* --- Environment as a dynamic NULL-terminated array --- */
typedef struct s_envarr
{
	char	**v;   /* array of "KEY=VALUE", always NULL-terminated */
	int		len;  /* number of entries (excluding the final NULL) */
	int		cap;  /* allocated capacity (excluding space for final NULL) */
}	t_envarr;

/* ---------- Shell state ---------- */

/* One node = one environment variable */
typedef struct s_env
{
	char           *key;   /* e.g. "PATH" (malloc'd, no '=')            */
	char           *val;   /* e.g. "/usr/bin:/bin" (malloc'd, may be NULL/empty) */
	struct s_env   *next;  /* next node                                  */
}	t_env;

typedef struct s_shell
{
    int         last_status;   // for $?
    t_env       *env_link; // linked list for env
    char        **env; // demoing
    t_input     input;
    char        **argv;          // arguments for current command
}   t_shell;

//SHELL 
int	shell_init(t_shell *shell, char *envp[]);

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
int	exec_pipeline(char ***argv_per_cmd, int cmd_count, char **envp);
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

// expansion.c 
int expand_tokens(t_input *input, int last_status);
char *expand_status(char *text, int last_status);
int	create_exp_status_text(char *text, char **exp_text, char *status_str);
int	process_quote_char(char c, int *in_single, int *in_double, char **exp_text);
int	process_status_str(char **exp_text, const char *status_str);
int	process_char(char **exp_text, char c);

#endif