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

typedef struct s_env
{
	char			*key;
	char			*val;
	struct s_env	*next;
}	t_env;

/* ---------- Shell state ---------- */

typedef struct s_shell
{
    int     last_status;   // for $?
    char  **env;           // environment variables (array)
    // or: t_env *env_list; if we manage as a linked list
    t_input	input;
    char 	**argv;          // arguments for current command
    char   *cwd;
}   t_shell;



//TODO move signals.h
extern volatile sig_atomic_t g_signal;
void	setup_signal_handlers_for_prompt();
void	setup_signal_handlers_for_child();
void	handle_sig(int signum);

//TODO move to executor.h
char	*join_cmd_to_path(const char *path, const char *cmd);
char	**find_from_path(char *envp[]);
char	**build_absolute_paths(char **paths, const char *cmd);
void	exec_ext_func_and_direct(char **absolute_paths, t_shell *shell, char *envp[]);
void	exec_direct(t_shell *shell, char *envp[]);
int	    has_slash(char *input); // helper in exec
int	    exec_pipe2(char **left_argv, char **right_argv, char *envp[]);
int	    run_line_minipipe(char *line, t_shell *shell, char *envp[]);

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