/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 07:03:12 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/08 18:23:35 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	has_slash(char *input)
{
	if (!input)
		return (0);
	while (*input)
		if (*input++ == '/')
			return (1);
	return (0);
}

static	void	child_close_all_pipes(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->pipeline.pipe_pair[0] >= 3)
		close (shell->pipeline.pipe_pair[0]);
	else if (shell->pipeline.pipe_pair[1] >= 3)
		close (shell->pipeline.pipe_pair[1]);
}

static void	execve_error_and_exit(t_shell *shell, char **argv,
									pid_t *child_pids, int saved_errno)
{
	if (child_pids)
		free(child_pids);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(argv[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (saved_errno == ENOENT || saved_errno == ENOTDIR)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		clean_env(&shell->env_head);
		free_split(&shell->env_arr);
		free_allocs(shell);
		_exit(127);
	}
	else
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		clean_env(&shell->env_head);
		free_split(&shell->env_arr);
		free_allocs(shell);
		_exit(126);
	}
}

static void	direct_exec(char **argv, t_shell *shell, pid_t *child_pids)
{
	int	err;

	execve(argv[0], argv, shell->env_arr);
	err = errno;
	execve_error_and_exit(shell, argv, child_pids, err);
}

static void	clean(char **directories, t_shell *shell, pid_t *child_pids)
{
	free_split(&directories);
	free_shell(shell);
	free(child_pids);
}

static void	path_exec(char **argv, t_shell *shell)
{
	char	path[PATH_MAX];
	char	*temp;
	char	*full;
	int		err;

	if (!argv || !argv[0])
		_exit(0);
	if (!getcwd(path, sizeof(path)))
		_exit(1);
	temp = ft_strjoin(path, "/");
	if (!temp)
		_exit(1);
	full = ft_strjoin(temp, argv[0]);
	free (temp);
	if (!full)
		_exit(1);
	execve(full, argv, shell->env_arr);
	err = errno;
	free (full);
	execve_error_and_exit(shell, argv, shell->pipeline.child_pids, err);
}

static void	exec_with_candidate_path(char **argv, char **path_dirs, t_shell *s)
{
	char	*candidate_path;
	int		path_index;

	path_index = 0;
	while (path_dirs[path_index])
	{
		candidate_path = join_cmd_to_path(path_dirs[path_index],
				argv[0]);
		if (!candidate_path)
		{
			clean(path_dirs, s, s->pipeline.child_pids);
			_exit(1);
		}
		execve(candidate_path, argv, s->env_arr);
		free(candidate_path);
		path_index++;
	}
}

static void	exec_with_path_search(int argc, char **argv, t_shell *shell)
{
	char	**path_directories;

	path_directories = NULL;
	if (argv && argv[0])
	{
		if (has_slash(argv[0]))
			direct_exec(argv, shell, shell->pipeline.child_pids);
		path_directories = find_from_path(shell->env_arr);
		if (!path_directories)
			path_exec(argv, shell);
	}
	if (argv && argv[0] && path_directories)
	{
		exec_with_candidate_path(argv, path_directories, shell);
	}
	if (argc != 0)
	{
		write(2, argv[0], (int)strlen(argv[0]));
		write(2, ": command not found\n", 20);
	}
	clean(path_directories, shell, shell->pipeline.child_pids);
	_exit(127);
}

static void	set_child_fds_from_pipes(int *final_in, int *final_out,
									int pipe_in, int pipe_out)
{
	*final_in = STDIN_FILENO;
	if (pipe_in >= 0)
		*final_in = pipe_in;
	*final_out = STDOUT_FILENO;
	if (pipe_out >= 0)
		*final_out = pipe_out;
}

static void	print_redir_error(t_redir *redir)
{
	if (redir && redir->target)
		perror(redir->target);
	else
		perror("redir");
}

static void	handle_redir_error(t_shell *shell, t_redir *redir)
{
	print_redir_error(redir);
	free_allocs(shell);
	free(shell->pipeline.child_pids);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
	_exit(1);
}

static void	handle_redir_out(t_redir *r, int *final_out, t_shell *shell)
{
	if (r && r->type == REDIR_OUT)
	{
		if (apply_redir_out(r, final_out) < 0)
			handle_redir_error(shell, r);
	}
}

static void	handle_redir_append(t_redir *r, int *final_out, t_shell *shell)
{
	if (r && r->type == REDIR_APPEND)
	{
		if (apply_redir_append(r, final_out) < 0)
			handle_redir_error(shell, r);
	}
}

static void	handle_redir_in(t_redir *r, int *final_in, t_shell *shell)
{
	if (r && r->type == REDIR_IN)
	{
		if (apply_redir_in(r, final_in) < 0)
			handle_redir_error(shell, r);
	}
}

static void	handle_redir_heredoc(t_redir *r, int *final_in, t_shell *shell)
{
	if (r && r->type == REDIR_HEREDOC)
	{
		if (apply_redir_heredoc(r, final_in) < 0)
			handle_redir_error(shell, r);
	}
}

static void	child_cleanup_and_exit(t_shell *shell, int status)
{
	free_allocs(shell);
	free(shell->pipeline.child_pids);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
	_exit(status);
}

static void	replug_child_stdin(int final_in)
{
	if (final_in != STDIN_FILENO)
	{
		if (dup2(final_in, STDIN_FILENO) < 0)
			_exit(1);
		close(final_in);
	}
}

static void	replug_child_stdout(int final_out)
{
	if (final_out != STDOUT_FILENO)
	{
		if (dup2(final_out, STDOUT_FILENO) < 0)
			_exit(1);
		close(final_out);
	}
}

static void	child_finalize_pipes(t_shell *shell)
{
	child_close_all_pipes(shell);
}

static void	maybe_run_builtin_and_exit(t_command *cmd, t_shell *shell)
{
	if (cmd && cmd->argv && cmd->argv[0] && is_builtin_name(cmd->argv[0]))
	{
		shell->last_status = run_builtin(cmd, shell);
		child_cleanup_and_exit(shell, shell->last_status);
	}
}

static void	process_all_redirs(t_list *redirs, int *final_in,
								int *final_out, t_shell *shell)
{
	t_list	*node;
	t_redir	*redir;

	node = redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		handle_redir_out(redir, final_out, shell);
		handle_redir_append(redir, final_out, shell);
		handle_redir_in(redir, final_in, shell);
		handle_redir_heredoc(redir, final_in, shell);
		node = node->next;
	}
}

pid_t	spawn_cmd(t_command *cmd, int pipe_in, int pipe_out, t_shell *shell)
{
	pid_t	pid;
	int		final_in;
	int		final_out;

	pid = fork();
	if (pid == 0)
	{
		shell->in_child = 1;
		setup_signal_handlers_for_child();
		set_child_fds_from_pipes(&final_in, &final_out, pipe_in, pipe_out);
		process_all_redirs(cmd->redirs, &final_in, &final_out, shell);
		replug_child_stdin(final_in);
		replug_child_stdout(final_out);
		child_finalize_pipes(shell);
		maybe_run_builtin_and_exit(cmd, shell);
		exec_with_path_search(cmd->argc, cmd->argv, shell);
		_exit(127);
	}
	return (pid);
}

int	apply_redir_out(const t_redir *redir, int *final_out)
{
	int	fd;

	if (!redir || !redir->target || !final_out)
		return (-1);
	fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	if (*final_out != STDOUT_FILENO && *final_out >= 0 && *final_out != fd)
		close(*final_out);
	*final_out = fd;
	return (0);
}

int	apply_redir_in(const t_redir *redir, int *final_in)
{
	int	fd;

	if (!redir || !redir->target || !final_in)
		return (-1);
	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
		return (-1);
	if (*final_in != STDIN_FILENO && *final_in >= 0 && *final_in != fd)
		close(*final_in);
	*final_in = fd;
	return (0);
}

int	apply_redir_append(const t_redir *redir, int *final_out)
{
	int	fd;

	if (!redir || !redir->target || !final_out)
		return (-1);
	fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	if (*final_out != STDOUT_FILENO && *final_out >= 0 && *final_out != fd)
		close(*final_out);
	*final_out = fd;
	return (0);
}

int	apply_redir_heredoc(const t_redir *redir, int *final_in)
{
	int	fd;

	if (!redir || !redir->target || !final_in)
		return (-1);
	fd = redir->hd_fd;
	if (fd < 0)
		return (-1);
	if (*final_in != STDIN_FILENO && *final_in >= 0 && *final_in != fd)
		close(*final_in);
	*final_in = fd;
	return (0);
}
