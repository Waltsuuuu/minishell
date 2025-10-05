#include "minishell.h"
/**
 * Checks whether a string contains a '/' character.
 *
 * @param input the string to scan (program name or path)
 * @return 1 if a slash is present, 0 otherwise
 */
static int	has_slash(char *input)
{
	if (!input)
		return (0);
	while (*input)
		if (*input++ == '/')
			return (1);
	return (0);
}

static	void child_close_all_pipes(t_shell *shell)
{

	if (!shell)
		return ;
		
	if (shell->pipeline.pipe_pair[0] >= 3)
		close (shell->pipeline.pipe_pair[0]);
			
	else if (shell->pipeline.pipe_pair[1] >= 3)
		close (shell->pipeline.pipe_pair[1]);

}

static void direct_exec(char **argv, t_shell *shell, pid_t *child_pids)
{
		execve(argv[0], argv, shell->env_arr);
		free(child_pids);
		if (errno == ENOENT || errno == ENOTDIR)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(argv[0], STDERR_FILENO);
			ft_putstr_fd(": No such file or directory\n", STDERR_FILENO);
			clean_env(&shell->env_head);
			free_split(&shell->env_arr);
			free_allocs(shell);
			_exit(127);
		}
		else
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(*argv, STDERR_FILENO);
			ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
			clean_env(&shell->env_head);
			free_split(&shell->env_arr);
			free_allocs(shell);
			_exit(126);
		}
}

static void	clean(char **directories, t_shell *shell, pid_t *child_pids)
{
	free_split(&directories);
	free_allocs(shell);
	free(child_pids);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
}

static void	path_exec(char **argv, t_shell *shell)
{
	char	path[PATH_MAX];
	char	*temp;
	char	*full;

	getcwd(path, sizeof(path)); 
	temp = ft_strjoin(path, "/");
	full = ft_strjoin(temp, argv[0]);
	free (temp);
	execve(full, argv, shell->env_arr);

}
//
static void	exec_with_path_search(int argc, char **argv, t_shell *shell)
{
	char	**path_directories;
	char	*candidate_path;
	int		path_index;

	if (argv && argv[0])
	{
    	if (has_slash(argv[0]))
    	{
        	direct_exec(argv, shell, shell->pipeline.child_pids);
        	return ; //TODO RETURN
    	}
    	path_exec(argv, shell);
	}

	path_directories = find_from_path(shell->env_arr);
	if (argv && argv[0] && path_directories)
	{
		path_index = 0;
		while (path_directories[path_index])
		{
			candidate_path = join_cmd_to_path(path_directories[path_index],
					argv[0]);
			if (!candidate_path)
			{	
				clean(path_directories, shell, shell->pipeline.child_pids);
				_exit(1);
			}
			execve(candidate_path, argv, shell->env_arr);
			free(candidate_path);
			path_index++;
		}
	}
	if (argc != 0)
	{
		write(2, argv[0], (int)strlen(argv[0]));
		write(2, ": command not found\n", 20);
	}
	clean(path_directories, shell, shell->pipeline.child_pids);
	_exit(127);
}


pid_t	spawn_cmd(t_command *cmd, int pipe_in, int pipe_out, t_shell *shell)
{
	pid_t	pid;
	int		final_in;
	int		final_out;
	t_list	*node;
	t_redir	*redir;

	pid = fork();
	if (pid == 0)
	{
		shell->in_child = 1; //Tag that we are in child
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
					//close_all_pipes(shell->pipeline.pipe_pair, shell->pipeline.n_cmds);
					free_allocs(shell);
					free(shell->pipeline.child_pids);
					//free(shell->pipeline.pipe_pair);
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
					//close_all_pipes(shell->pipeline.pipe_pair, shell->pipeline.n_cmds);
					free_allocs(shell);
					free(shell->pipeline.child_pids);
					//free(shell->pipeline.pipe_pair);
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
					//close_all_pipes(shell->pipeline.pipe_pair, shell->pipeline.n_cmds);
					free_allocs(shell);
					free(shell->pipeline.child_pids);
					//free(shell->pipeline.pipe_pair);
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
					//close_all_pipes(shell->pipeline.pipe_pair, shell->pipeline.n_cmds);
					free_allocs(shell);
					free(shell->pipeline.child_pids);
					//free(shell->pipeline.pipe_pair);
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
	
		child_close_all_pipes(shell);
		if (cmd && cmd->argv && cmd->argv[0] //betarunning builtins
			&& is_builtin_name(cmd->argv[0]))
		{
			shell->last_status = run_builtin(cmd, shell);
			//close_all_pipes(shell->pipeline.pipe_pair, shell->pipeline.n_cmds);
			free_allocs(shell);
			free(shell->pipeline.child_pids);
			//free(shell->pipeline.pipe_pair);
			clean_env(&shell->env_head);
			free_split(&shell->env_arr);
			_exit(shell->last_status);
		}

		/* 4. Aja komento */
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

int apply_redir_append(const t_redir *redir, int *final_out)
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

