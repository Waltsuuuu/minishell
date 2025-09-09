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
static void	exec_with_path_search(char **argv, char **envp)
{
	char	**path_directories;
	char	*candidate_path;
	int		path_index;

	if (argv && argv[0] && has_slash(argv[0]))
	{
		execve(argv[0], argv, envp);
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
				free_split(path_directories);
				_exit(1);
			}
			execve(candidate_path, argv, envp);
			free(candidate_path);
			path_index++;
		}
		free_split(path_directories);
	}
	write(2, "minishell: ", 11);
	if (argv && argv[0])
		write(2, argv[0], (int)strlen(argv[0]));
	write(2, ": command not found\n", 20);
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
 */
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