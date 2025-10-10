/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:42:59 by wheino            #+#    #+#             */
/*   Updated: 2025/10/10 14:03:20 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTE_H
# define EXECUTE_H

# include "minishell.h"

typedef struct s_shell		t_shell;
typedef struct s_pipeline	t_pipeline;
typedef struct s_command	t_command;
typedef struct s_redir		t_redir;

typedef struct s_exec
{
	int					cmd_index;
	int					previous_read;
	int					next_read;
	int					next_write;
	struct sigaction	ign;
	struct sigaction	old_quit;
	struct sigaction	old_int;
	struct termios		tty;
}	t_exec;

int		exec_dispatch(t_pipeline *pipeline, t_shell *shell);
char	*join_cmd_to_path(const char *path, const char *cmd);
char	**find_from_path(char *envp[]);
char	**build_absolute_paths(char **paths, const char *cmd);
int		exec_pipeline(t_pipeline *pipeline, t_shell *shell);
void	compute_cmd_fds(int cmd_index, t_pipeline *pipeline,
			int	*in_fd, int *out_fd);
int		open_next_pipe_if_needed(t_shell *shell, t_exec *exec);
pid_t	spawn_cmd(t_command *cmd, int pipe_in, int pipe_out, t_shell *shell);
int		wait_for_pid_once(pid_t target_pid, int *out_raw_status);
int		wait_all_and_last_status( int child_count,
			pid_t last_child_pid);
int		apply_redir_out(const t_redir *r, int *final_out);
int		apply_redir_append(const t_redir *redir, int *final_out);
int		apply_redir_in(const t_redir *redir, int *final_in);
int		apply_redir_heredoc(const t_redir *redir, int *final_in);
int		run_builtin(t_command *cmd, t_shell *shell);
int		alloc_child_pids(t_pipeline *pl);
void	init_parent_pipe_pair(t_shell *shell);
void	close_if_nonneg(int fd);
int		on_open_pipe_error(pid_t *pids, t_exec *exec);
int		on_spawn_error(t_pipeline *pl, t_exec *exec);
void	print_redir_error(t_redir *redir);
void	handle_redir_heredoc(t_redir *r, int *final_in, t_shell *shell);
void	handle_redir_in(t_redir *r, int *final_in, t_shell *shell);
void	handle_redir_append(t_redir *r, int *final_out, t_shell *shell);
void	handle_redir_out(t_redir *r, int *final_out, t_shell *shell);
void	child_finalize_pipes(t_shell *shell);
void	replug_child_stdout(int final_out);
void	replug_child_stdin(int final_in);
void	set_child_fds_from_pipes(int *final_in, int *final_out,
			int pipe_in, int pipe_out);
void	child_close_all_pipes(t_shell *shell);
void	direct_exec(char **argv, t_shell *shell, pid_t *child_pids);
void	path_exec(char **argv, t_shell *shell);
void	exec_with_candidate_path(char **argv, char **path_dirs, t_shell *s);
void	exec_with_path_search(int argc, char **argv, t_shell *shell);

#endif