#include "minishell.h"

// Initiazlies the collect_heredoc_body() variables to 0, NULL or -1 (not set).
void	init_hd_state(t_hd_state *state)
{
	ft_bzero(state, sizeof(state));
	state->fds[0] = -1;
	state->fds[1] = -1;
	state->line = NULL;
}

// Closes both ends of the pipe and returns -1
// Only use on error route.
int		close_pipe_err(t_hd_state *state)
{
	close(state->fds[0]);
	close(state->fds[1]);
	return (-1);
}

// Frees the input line and closes both ends of pipe.
void	free_line_close_fds(int fds[2], char *line)
{
	free(line);
	close(fds[0]);
	close(fds[1]);	
}

// Wrapper for restoring both tty and parent signal handlers
// back to their original state before fork().
void	restore_tty_and_sig(t_hd_state *state)
{
	restore_terminal_state(state);
	restore_parent_sig_handlers(state);
}