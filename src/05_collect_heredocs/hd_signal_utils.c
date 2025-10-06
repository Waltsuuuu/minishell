#include "minishell.h"

// Ignore SIGINT and SIGQUIT in parent during waitpid().
void	ignore_parent_sig_handlers(t_hd_state *state)
{
	ft_bzero(&state->ign, sizeof(state->ign));
	state->ign.sa_handler = SIG_IGN;
	sigemptyset(&state->ign.sa_mask);
	sigaction(SIGINT,  &state->ign, &state->old_int);
	sigaction(SIGQUIT, &state->ign, &state->old_quit);
}

void	heredoc_child_sighandler(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = close_stdin_on_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	close_stdin_on_sigint(int sig_num)
{
	g_signal = sig_num;
	close(STDIN_FILENO);
}

// Waits for the child process to finish, retrying if interrupted by a signal. 
int	wait_child(t_hd_state *state)
{
	while (1)
	{
		state->wait_result = waitpid(state->pid, &state->status, 0);
		if (state->wait_result == -1 && errno == EINTR) // Interrupted signal call.
			continue ;							 		//  - Retry.
		break;									 		// Success or real error.
	}
	if (state->wait_result == -1)						// waitpid() error (not EINTR).
		return (-1);									// 	- Return error.
	return (0);											// Return success.
}

// Restore SIGINT and SIGQUIT handlers in parent (stop ignoring) after waitpid().
void	restore_parent_sig_handlers(t_hd_state *state)
{
	sigaction(SIGINT,  &state->old_int,  NULL);
	sigaction(SIGQUIT, &state->old_quit, NULL);
}

// Interprets child status: 
// if SIGINT or nonzero exit - returns -1;
// otherwise returns 0.
int	handle_child_status(t_hd_state *state, t_shell *shell)
{
	if (WIFSIGNALED(state->status) && WTERMSIG(state->status) == SIGINT)	// Child terminated by signal && Signal was SIGINT.
	{
		close(state->fds[0]);
		shell->last_status = 130;
		return (-1);
	}
	if (!WIFEXITED(state->status) || WEXITSTATUS(state->status) != 0)		// Normal exit (WIFEXITED = true), but non-zero (failure).
	{
		close(state->fds[0]);
		return (-1);
	}
	return (0);
}
