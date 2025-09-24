#ifndef MINISHELL_H
# define MINISHELL_H

#include "libft.h"
# include "printf/ft_printf.h"
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
# include <fcntl.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
    int			last_status;   
    t_env		*env_head;
    t_input		input;
    char		*cwd;
	t_pipeline	pipeline;
	char		*buf;
}   t_shell;

typedef struct s_expand_state
{
	int	i;
	int	in_single;
	int	in_double;
	int	quote_handled;
	int	expanded;
}	t_expand_state;



// 00_TOKENIZE
/*					See tokenizer.h 							*/

// 01_BALANCE_QUOTES
/* 					quote_check.c								*/
int check_quote_balance(char **line, int *last_status);
int	quotes_unbalanced(const char *string);

// 02_EXPAND_VARS_AND_STATUS
/* 					01_expansion.c								*/
int		expand_tokens(t_input *input, int last_status, char **envp);

/* 					02_expand_status.c							*/
char	*expand_status(char *text, int last_status);
int		create_exp_status_text(char *text, char **exp_text, char *status_str);

/* 					03_expand_variable.c						*/
char	*expand_variable(char *text, char **envp);
int		create_exp_var_text(char *text, char **exp_text, char **envp);
int		handle_var_expansion(char *text, char **exp_text, int *i, char **envp, int in_single);
int		process_var_expansion(char *text, char **exp_text, int *i, char **envp);

/* 					04_expand_utils.c							*/
int		valid_cont_char(char c);
int		valid_start_char(char c);
int		process_expanded_str(char **exp_text, const char *str);
int		process_quote_char(char c, int *in_single, int *in_double, char **exp_text);
int		process_char(char **exp_text, char c);

/* 					05_expand_utils2.c							*/
void	init_expand_state(t_expand_state *st);
size_t	copy_n_chars(char *dst, const char *src, size_t size);
int		extract_key(char *text, int *i, char **key, int *key_len, int *start_i);
int		find_env_index(char **envp, char *key, int key_len);

// 03_REMOVE_QUOTES
/*					quote_removal.c								*/
int		remove_quotes(t_input *input);
char	*handle_quote_removal(char *text, int *was_quoted);
int		create_unquoted_text(char *text, char **unquoted_text, int *was_quoted);
int		remove_outer_quote(char c, int *in_single, int *in_double);

// 04_BUILD_CMD_PIPELINE
/*					See pipeline.h								*/

// 05_EXECUTE_CMD_PIPELINE
int		exec_dispatch(char **envp, t_pipeline *pipeline, t_shell *shell);
char	*join_cmd_to_path(const char *path, const char *cmd);
char	**find_from_path(char *envp[]);
char	**build_absolute_paths(char **paths, const char *cmd);
int	    has_slash(char *input); // helper in exec
int		exec_pipeline(char **envp, t_pipeline *pipeline, t_shell *shell);
void	close_parent_unused_ends(int stage_index, int cmd_count,
int		(*pipe_pairs)[2]);
void	compute_cmd_fds(int cmd_index, int cmd_count, int (*pipe_pairs)[2],
int 	*in_fd, int *out_fd);
void	close_all_pipes(int (*pipe_pairs)[2], int cmd_count);
int		(*allocate_pipes(int cmd_count))[2];
pid_t	spawn_cmd(t_command *cmd, char **envp, int pipe_in, int pipe_out, t_shell *shell, pid_t *child_pids, int (*pipe_pairs)[2]);
int		apply_redir_out(const t_redir *r, int *final_out);
int		wait_for_pid_once(pid_t target_pid, int *out_raw_status);
int		wait_all_and_last_status(pid_t *child_pids, int child_count,
		pid_t last_child_pid);
int		apply_redir_out(const t_redir *r, int *final_out);
int 	apply_redir_append(const t_redir *redir, int *final_out);
int		apply_redir_in(const t_redir *redir, int *final_in);

// UTILS
/*					get_working_dir.c							*/
void	getworkindir(char *buf, size_t size);

/*					free_allocs.c								*/
void	free_allocs(t_shell *shell);

/*					print_msh_banner.c							*/
void	print_msh_banner(void);

/*					free_split.c								*/
void	free_split(char **arr);
void	free_partial(char **arr, size_t n);

// UTILS - signal_handling
/*					signal.c									*/
extern volatile sig_atomic_t g_signal;
void	setup_signal_handlers_for_prompt();
void	setup_signal_handlers_for_child();
void	handle_sig(int signum);
/*					ENV											*/
t_env	*env_init_from_envp(char **envp);
t_env	*create_new_env_node(const char *key, const char *value);
int		append_env_node(t_env **head, t_env *new_env_node);
void	clean_env(t_env **head);
char	**env_list_to_array(t_env *head, int *size);
char	*ft_strjoin_with_equal_sign(char const *s1, char const *s2);
void	print_env(t_shell *shell);
void	env_sort_and_print(t_shell *shell);
int		find_equal_sign(char *str);

/*					Builtins									*/
int		is_builtin_name(const char *name);
int		builtin_export(char **argv, t_shell *shell);
int		try_run_single_builtin(t_pipeline *pipeline, t_shell *shell);
int		split_key_and_value(char *line, char **key_out, char **value_out);
int		exec_export_in_parent(t_command *cmd, t_shell *shell);
int  	process_export_arg(char *arg, t_shell *shell);

int		is_builtin_valid(const char *key);
void	print_invalid_identifier(const char *builtin, const char *key);
int		env_set(t_env **head, const char *key, const char *value);
t_env	*env_find(t_env *head, const char *key);
int		find_equal_sign(char *str);
int		is_single_export(const t_pipeline *p);
int		save_stdio(int saved[2]);
void	restore_stdio(int saved[2]);
int		apply_redirs_in_parent(t_command *cmd, int saved[2]);


#endif