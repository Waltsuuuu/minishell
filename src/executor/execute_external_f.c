
#include "minishell.h"

int	status_from_wait(int wstatus)
{
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	return (1);
}

void	exec_ext_func_and_direct(char **absolute_paths, t_shell *shell, char *envp[])
{
	size_t	counter;
	pid_t	pid;
	int 	wstatus;
	//Checking for direct exec
	if (shell->input.words && shell->input.words[0]
		&& has_slash(shell->input.words[0]))
	{
		exec_direct(shell);
		return;
	}

	counter = 0;
		while (absolute_paths && absolute_paths[counter])
		{
			if (access(absolute_paths[counter], X_OK) == 0)
			{
				pid = fork();
				if (pid == 0)
				{
					setup_signal_handlers_for_child();
					execve(absolute_paths[counter], shell->input.words, envp);
					perror("execve");
					_exit(127);
				}
				waitpid(pid, &wstatus, 0);
				shell->last_status = status_from_wait(wstatus);	//TODO add after we get the struct
				break;
			}
			counter++;
		}
		if (!absolute_paths || !absolute_paths[counter])
		{
			write(2, "minishell: command not found\n", 29);
			shell->last_status = 127; //TODO Signal hardcoded here for now
		}
}

int	has_slash(char *input)
{
	while (*input)
		if (*input++ == '/')
			return (1);
	return (0);
}

void	exec_direct(t_shell *shell)
{
	pid_t	pid;
	int		wstatus;
	char	*path;

	path = shell->input.words[0];

	pid = fork();
	if (pid == 0)
	{
		setup_signal_handlers_for_child();
		execve(path, &shell->input.words[0] , shell->env);
		perror("execve");
		_exit(127);
	}
	waitpid(pid, &wstatus, 0);
		shell->last_status = status_from_wait(wstatus);
}