#include "minishell.h"

/**
 * Executes a command by resolving its path.
 * If argv[0] contains '/', executes directly
 * Otherwise searches PATH for a matching executable and execs the first hit.
 * On failure, prints "command not found" and exits with 127.
 *
 * @param argv the command and its arguments (argv[0] is the program name)
 * @param envp the environment vector
 * @return never returns on success; calls _exit(126/127/1) on failure
 *
 * @note Exits with 127 for ENOENT/ENOTDIR when using a direct path,
 *       126 for other execve errors, and 127 if not found in PATH.
 */
static void	exec_with_path_search(char **argv, char **envp, t_shell *shell, pid_t *child_pids, int (*pipe_pairs)[2])
{
	char	**path_directories;
	char	*candidate_path;
	int		path_index;

	if (argv && argv[0] && has_slash(argv[0]))
	{
		execve(argv[0], argv, envp);
		free_allocs(shell);
		free(child_pids);
		free(pipe_pairs);
		clean_env(&shell->env_head);
		free_split(&shell->env_arr);
		_exit((errno == ENOENT || errno == ENOTDIR) ? 127 : 126); 
	}
	path_directories = find_from_path(envp);
	if (argv && argv[0] && path_directories)
	{
		path_index = 0;
		while (path_directories[path_index])
		{
			candidate_path = join_cmd_to_path(path_directories[path_index],
					argv[0]);
			if (!candidate_path)
			{
				free_split(&path_directories);
				free_allocs(shell);
				free(child_pids);
				free(pipe_pairs);
				clean_env(&shell->env_head);
				free_split(&shell->env_arr);
				_exit(1);
			}
			execve(candidate_path, argv, envp);
			free(candidate_path);
			path_index++;
		}
		free_split(&path_directories);
	}
	write(2, "minishell: ", 11);
	if (argv && argv[0])
		write(2, argv[0], (int)strlen(argv[0]));
	write(2, ": command not found\n", 20);
	free_allocs(shell);
	free(child_pids);
	free(pipe_pairs);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
	free_split(&path_directories);
	_exit(127);
}

/**
 * Forks and executes a command with optional stdin/stdout redirection.
 * In the child: sets signal handlers, duplicates in_fd/out_fd (if >= 0),
 * then calls exec_with_path_search(argv, envp). On dup2 failure, exits(1).
 *
 * @param argv the command and its arguments
 * @param envp the environment vector
 * @param in_fd file descriptor to use as STDIN (pass -1 to keep current)
 * @param out_fd file descriptor to use as STDOUT (pass -1 to keep current)
 * @return PID of the child in the parent process; 0 in the child; -1 on fork error
 *
pid_t	spawn_cmd(char **argv, char **envp, int in_fd, int out_fd)
{
	pid_t	child_pid;

	child_pid = fork();
	if (child_pid == 0)
	{
		setup_signal_handlers_for_child();
		if (in_fd >= 0 && dup2(in_fd, 0) < 0)
			_exit(1);
		if (out_fd >= 0 && dup2(out_fd, 1) < 0)
			_exit(1);
		exec_with_path_search(argv, envp);
	}
	return (child_pid);
}*/

pid_t	spawn_cmd(t_command *cmd, char **envp, int pipe_in, int pipe_out, t_shell *shell, pid_t *child_pids, int (*pipe_pairs)[2])
{
	pid_t	pid;
	int		final_in;
	int		final_out;
	t_list	*node;
	t_redir	*redir;

	pid = fork();
	if (pid == 0)
	{
		setup_signal_handlers_for_child();

		// 1. Aloitus oletuksilla (putkista jos on, muuten stdin/stdout)
		final_in = STDIN_FILENO;
		if (pipe_in >= 0)
			final_in = pipe_in;
		final_out = STDOUT_FILENO;
		if (pipe_out >= 0)
			final_out = pipe_out;

		// 2. Käy läpi redirit: tässä vaiheessa vain '>'
		node = cmd->redirs;
		while (node)
		{
			redir = (t_redir *)node->content;

			if (redir && redir->type == REDIR_OUT) 
			{
				if (apply_redir_out(redir, &final_out) < 0)  // Tarkistetaan onnnistuisko avaus
				{
    				if (redir->target) // Tarkistetaan onko kohdenimi olemassa 
        				perror(redir->target);
    				else
					{
        				perror("redir");
					}
					close_all_pipes(pipe_pairs, shell->pipeline.n_cmds);
					free_allocs(shell);
					free(child_pids);
					free(pipe_pairs);
					clean_env(&shell->env_head);
					free_split(&shell->env_arr);
					_exit(1);
				}

			}

			if (redir && redir->type == REDIR_APPEND)
			{
				if (apply_redir_append(redir, &final_out) < 0)
				{
					if (redir->target)
						perror(redir->target);
					else
						perror("redir");
					close_all_pipes(pipe_pairs, shell->pipeline.n_cmds);
					free_allocs(shell);
					free(child_pids);
					free(pipe_pairs);
					clean_env(&shell->env_head);
					free_split(&shell->env_arr);
					_exit(1);
				}
			}

			if (redir && redir->type == REDIR_IN)
			{
				if (apply_redir_in(redir, &final_in) < 0)
				{
					if (redir->target)
						perror(redir->target);
					else
						perror("redir");
					close_all_pipes(pipe_pairs, shell->pipeline.n_cmds);
					free_allocs(shell);
					free(child_pids);
					free(pipe_pairs);
					clean_env(&shell->env_head);
					free_split(&shell->env_arr);	
					_exit(1);
				}
			}

			if (redir && redir->type == REDIR_HEREDOC)
			{
				if (apply_redir_heredoc(redir, &final_in) < 0)
				{
					if (redir->target)
						perror(redir->target);
					else
						perror("redir");
					close_all_pipes(pipe_pairs, shell->pipeline.n_cmds);
					free_allocs(shell);
					free(child_pids);
					free(pipe_pairs);
					clean_env(&shell->env_head);
					free_split(&shell->env_arr);
					_exit(1);
				}
			}

			node = node->next;
		}
	

		/* 3. Tee dup2 lopullisille fd:ille ja sulje ylimääräiset */
		if (final_in != STDIN_FILENO)
		{
			if (dup2(final_in, STDIN_FILENO) < 0)
				_exit(1);
			close(final_in);
		}
		if (final_out != STDOUT_FILENO)
		{
			if (dup2(final_out, STDOUT_FILENO) < 0)
				_exit(1);
			close(final_out);
		}
	
		close_all_pipes(pipe_pairs, shell->pipeline.n_cmds);

		if (cmd && cmd->argv && cmd->argv[0]
			&& ft_strcmp(cmd->argv[0], "export") == 0)
		{
			shell->last_status = builtin_export(cmd->argv, shell);
			free_allocs(shell);
			free(child_pids);
			free(pipe_pairs);
			clean_env(&shell->env_head);
			free_split(&shell->env_arr);
			_exit(shell->last_status);
		}

		/* 4. Aja komento */
		exec_with_path_search(cmd->argv, envp, shell, child_pids, pipe_pairs);
		if (cmd->argv && cmd->argv[0])
    		perror(cmd->argv[0]);
		else
    		perror("execve");
		_exit(127);
	}
	return (pid);
}
/*
static int  open_one_redir(const t_redir *redir)
{
    int fd;

    fd = -1;
    if (redir->type == REDIR_IN)
        fd = open(redir->target, O_RDONLY);
    else if (redir->type == REDIR_OUT)
        fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    else if (redir->type == REDIR_APPEND)
        fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	//heredoc here too?
    return (fd);
}
*/

int	apply_redir_out(const t_redir *redir, int *final_out)
{
	int	fd;

	if (!redir || !redir->target || !final_out)
		return (-1);
	fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
		/* Jos final_out on jo ollut joku muu fd kuin STDOUT,
	   ja se ei ole sama kuin juuri avattu fd,
	   sulje se, ettei jää fd-vuotoa. */
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
		/* Jos final_out on jo ollut joku muu fd kuin STDOUT,
	   ja se ei ole sama kuin juuri avattu fd,
	   sulje se, ettei jää fd-vuotoa. */
	if (*final_in != STDIN_FILENO && *final_in >= 0 && *final_in != fd)
		close(*final_in);
	*final_in = fd;
	return (0);
}

int apply_redir_append(const t_redir *redir, int *final_out)
{
	int	fd;

	if (!redir || !redir->target || !final_out)
		return (-1);
	fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
		/* Jos final_out on jo ollut joku muu fd kuin STDOUT,
	   ja se ei ole sama kuin juuri avattu fd,
	   sulje se, ettei jää fd-vuotoa. */
	if (*final_out != STDOUT_FILENO && *final_out >= 0 && *final_out != fd)
		close(*final_out);
	*final_out = fd;
	return (0);
}

int	apply_redir_heredoc(const t_redir *redir, int *final_in)
{
	int fd;

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

/**
 * Checks whether a string contains a '/' character.
 *
 * @param input the string to scan (program name or path)
 * @return 1 if a slash is present, 0 otherwise
 */
int	has_slash(char *input)
{
	while (*input)
		if (*input++ == '/')
			return (1);
	return (0);
}