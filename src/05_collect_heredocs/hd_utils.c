#include "minishell.h"

void	init_hd_state(t_hd_state *state)
{
	ft_bzero(state, sizeof(state));
	state->fds[0] = -1;
	state->fds[1] = -1;
	state->line = NULL;
}

int		close_pipe_err(t_hd_state *state)
{
	close(state->fds[0]);
	close(state->fds[1]);
	return (-1);
}

void	free_line_close_fds(int fds[2], char *line)
{
	free(line);
	close(fds[0]);
	close(fds[1]);	
}

void	restore_tty_and_sig(t_hd_state *state)
{
	restore_terminal_state(state);
	restore_parent_sig_handlers(state);
}