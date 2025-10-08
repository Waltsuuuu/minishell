#include "minishell.h"

/**
 * @brief Loops over all commands inside t_pipeline->cmds,
 * and calls collect_cmd_heredocs() for each command.
 * @return 0 on success, -1 on error.
 */
int	collect_heredocs(t_pipeline *pipeline, t_shell *shell, char **envp)
{
	int	cmd_i;

	if (!pipeline || !pipeline->cmds || pipeline->n_cmds <= 0)
		return (-1);
	cmd_i = 0;
	while (cmd_i < pipeline->n_cmds)
	{
		if (collect_cmd_heredocs(&pipeline->cmds[cmd_i], shell, envp) == -1)
			return (-1);
		cmd_i++;
	}
	return (0);
}

/**
 * @brief Loops over all redirects for a single command,
 * searching for a REDIR_HEREDOC.
 * If found, calls collect_heredoc_body().
 * @return 0 on success, -1 on error.
 */
int	collect_cmd_heredocs(t_command *cmd, t_shell *shell, char **envp)
{
	t_list 	*current;
	t_redir	*redir;

	current = cmd->redirs;
	while (current)
	{
		redir = (t_redir *)current->content;
		if (redir && redir->type == REDIR_HEREDOC)
		{
			if (collect_heredoc_body(redir, shell, envp) == -1)
				return (-1);
		}
		current = current->next;
	}
	return (0);						
}
/**
 * @brief Prompts and collects new input from user.
 * New input is written into a pipe.
 * The read end of the pipe (fd[0]) is stored as t_redir->hd_fd.
 * New input is collected in a child process, via fork_and_collect_hd().
 * A snapshot of the tty state is taken prior to forking,
 * and restored once the child has returned.
 * While child process is running, parent sig handlers are disabled,
 * and restored once the child returns.
 * @return 0 on success, -1 on error.
 */
int	collect_heredoc_body(t_redir *redir, t_shell *shell, char **envp)
{
	t_hd_state	state;

	init_hd_state(&state);
	save_terminal_state(&state.tty);
	if (pipe(state.fds) == -1)
		return (-1);
	if (fork_and_collect_hd(&state, shell, redir, envp) == -1)
	{
		restore_terminal_state(&state.tty);
		return(close_pipe_err(&state));
	}
	close(state.fds[1]);
	ignore_parent_sig_handlers(&state.ign, &state.old_quit, &state.old_int);
	if (wait_child(&state) == -1)
	{
		restore_tty_and_sig(&state);
		close(state.fds[0]);
		return (-1);
	}
	restore_tty_and_sig(&state);
	if (handle_child_status(&state, shell) == -1)
		return (-1);
	redir->hd_fd = state.fds[0];
	return (0);
}

/**
 * @brief Forks and collects the new input in a child process.
 * @return Exits on success, returns -1 on fork error.
 */
int	fork_and_collect_hd(t_hd_state *state, t_shell *shell, t_redir *redir, char **envp)
{
	state->pid = fork();
	if (state->pid < 0)
		return(-1);
	if (state->pid == 0)
		collect_hd(state, shell, redir, envp);
	return(0);
}

int	collect_hd(t_hd_state *state, t_shell *shell, t_redir *redir, char **envp)
{
	heredoc_child_sighandler();
	close(state->fds[0]);
	while (g_signal == 0)
	{
		if (readline_and_check_eof(state, redir) == 1)
			break ;
		if (handle_heredoc_line(state, state->fds[1], state->line, redir, shell->last_status, envp) == -1)
		{
			free(state->line);
			close(state->fds[1]);
			free_shell(shell);
			_exit(1);
		}
		free(state->line);
	}
	free_shell(shell);
	close(state->fds[1]);
	if (g_signal == SIGINT)
	{
		signal(SIGINT, SIG_DFL);
		kill(getpid(), SIGINT);
		_exit(130);
	}
	_exit(0);
}
