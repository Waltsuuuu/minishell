/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 07:03:12 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 09:13:14 by mhirvasm         ###   ########.fr       */
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

void	print_redir_error(t_redir *redir)
{
	if (redir && redir->target)
		perror(redir->target);
	else
		perror("redir");
}

static void	child_cleanup_and_exit(t_shell *shell, int status)
{
	free_allocs(shell);
	free(shell->pipeline.child_pids);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
	_exit(status);
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

