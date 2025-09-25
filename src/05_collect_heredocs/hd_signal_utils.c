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

// In child - Set SIGINT and SIGQUIT handlers to default.
void	set_default_sig_handling(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = SIG_DFL;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
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
