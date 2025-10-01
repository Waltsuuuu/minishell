#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "printf/ft_printf.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/stat.h> // access and macros
# include <sys/types.h>
# include <sys/wait.h>
# include <signal.h>
# include <errno.h>
# include <fcntl.h>
# include <termios.h>
# include "colors.h"
# include "tokenize.h"
# include "expand.h"
# include "pipeline.h"
# include "heredoc.h"
# include "utils.h"
# include "quotes.h"
# include "../libft/get_next_line/get_next_line.h"
# include "limits.h"

extern volatile sig_atomic_t g_signal;

typedef struct s_env
{
	char			*key;
	char			*value;
	int				assigned;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
    int			last_status;   
    t_env		*env_head;
	char		**env_arr;
	int			env_size;
    t_input		input;
    char		*cwd;
	t_pipeline	pipeline;
	char		*buf;
	int			in_child;
	int			interactive;
}   t_shell;

// 00_BALANCE_QUOTES
/*					See quotes.h 								*/

// 01_TOKENIZE
/*					See tokenizer.h 							*/

// 02_EXPAND_VARS_AND_STATUS
/*					See expand.h 								*/

// 03_REMOVE_QUOTES
/*					See quotes.h 								*/

// 04_BUILD_CMD_PIPELINE
/*					See pipeline.h								*/

// 05_COLLECT_HEREDOCS
/*					See heredoc.h								*/

// 06_EXECUTE_CMD_PIPELINE
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
int		apply_redir_heredoc(const t_redir *redir, int *final_in);
int		run_builtin(t_command *cmd, t_shell *shell);

// 07_BUILTINS
int		is_builtin_name(const char *name);
int		builtin_export(char **argv, t_shell *shell);
int		split_key_and_value(char *line, char **key_out, char **value_out);
int		exec_export_in_parent(t_command *cmd, t_shell *shell);
int  	process_export_arg(char *arg, t_shell *shell);
int		env_list_to_export_display_array(t_env *head, t_shell *shell);
int		is_builtin_valid(const char *key);
void	print_invalid_identifier(const char *builtin, const char *key);
int		env_set(t_env **head, const char *key, const char *value);
t_env	*env_find(t_env *head, const char *key);
int		find_equal_sign(char *str);
int		save_stdio(int saved[2]);
void	restore_stdio(int saved[2]);
int		apply_redirs_in_parent(t_command *cmd, int saved[2]);
int		is_parent_builtin(const char *name);
int		exec_unset_in_parent(t_command *cmd, t_shell *shell);
int		builtin_unset(char **argv, t_shell *shell);
int		env_unset(t_shell *shell, const char *key);
int		builtin_cd(char **argv, t_shell *shell);
int		exec_cd_in_parent(t_command *cmd, t_shell *shell);
int		builtin_pwd(void);
int		builtin_echo(t_command *cmd);
int 	exec_exit_in_parent(t_command *cmd, t_shell *shell);
int		exit_core(t_command *cmd, t_shell *shell);

/*					ENV											*/
t_env	*env_init_from_envp(char **envp);
t_env	*create_new_env_node(const char *key, const char *value);
int		append_env_node(t_env **head, t_env *new_env_node);
void	clean_env(t_env **head);
int		env_list_to_array(t_env *head, t_shell *shell);
char	*ft_strjoin_with_equal_sign(char const *s1, char const *s2);
void	print_env(t_shell *shell);
void	env_sort_and_print(t_shell *shell);
int		find_equal_sign(char *str);
char	*env_get(t_shell *shell, const char *key);

// UTILS
/*					See utils.h									*/


#endif