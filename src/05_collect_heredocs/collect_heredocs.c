#include "minishell.h"

// Loops through commands
int	collect_heredocs(t_pipeline *pipeline, t_shell *shell, char **envp)
{
	int	cmd_i;

	if (!pipeline || !pipeline->cmds || pipeline->n_cmds <= 0)
		return (-1);
	cmd_i = 0;
	while (cmd_i < pipeline->n_cmds)								// Loop over all commands
	{
		if (collect_cmd_heredocs(&pipeline->cmds[cmd_i], shell, envp) == -1)		// Loop over the redirects of the command and check for heredocs.
			return (-1);
		cmd_i++;													// Move to next command.
	}
	return (0);
}
// Loops all redirs in a command (looking for heredoc redir)
int	collect_cmd_heredocs(t_command *cmd, t_shell *shell, char **envp)
{
	t_list 	*current;
	t_redir	*redir;

	current = cmd->redirs;
	while (current)										// Loop over the kinked list of redirects for the current command.
	{
		redir = (t_redir *)current->content;
		if (redir && redir->type == REDIR_HEREDOC)		// If the redir type is heredoc, go collect the body.
		{
			if (collect_heredoc_body(redir, shell, envp) == -1)		// 	-Collect heredoc body.
				return (-1);
		}
		current = current->next;						// Move to next redirect.
	}
	return (0);											
}
// Collects the heredoc body for one heredoc.
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
	close(state.fds[1]);  // Parent only reads
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

// Forks and collects the heredoc body in a child process.
// Sets SIGINT and SIGQUIT handling back the default.
int	fork_and_collect_hd(t_hd_state *state, t_shell *shell, t_redir *redir, char **envp)
{
	state->pid = fork();
	if (state->pid < 0)
		return(-1);
	if (state->pid == 0)
	{
		heredoc_child_sighandler();
		close(state->fds[0]); //Child only writes.
		while (g_signal == 0)
		{
			if (readline_and_check_eof(state, redir) == 1)
				break ;
			if (handle_heredoc_line(state, state->fds[1], state->line, redir, shell->last_status, envp) == -1)
			{
				free(state->line);
				close(state->fds[1]);
				free_allocs(shell);
				free_split(&shell->env_arr);
				clean_env(&shell->env_head);
				_exit(1);
			}
			free(state->line);
		}
		free_allocs(shell);
		free_split(&shell->env_arr);
		clean_env(&shell->env_head);
		close(state->fds[1]);
		if (g_signal == SIGINT)
		{
			signal(SIGINT, SIG_DFL);
			kill(getpid(), SIGINT);
			_exit(130);
		}
		_exit(0);
	}
	return(0);
}
