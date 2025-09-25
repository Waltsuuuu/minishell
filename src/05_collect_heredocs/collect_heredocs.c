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
	while (current)										// Loop over the redirect linked list of this specific command
	{
		redir = (t_redir *)current->content;
		if (redir && redir->type == REDIR_HEREDOC)		// If the redir type is heredoc, go collect the body
		{
			if (collect_heredoc_body(redir, shell, envp) == -1)		// 		-Collect heredoc body.
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
		restore_terminal_state(&state);
		return(close_pipe_err(&state));
	}
	close(state.fds[1]);  // Parent only reads
	ignore_parent_sig_handlers(&state);
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

void	restore_tty_and_sig(t_hd_state *state)
{
	restore_terminal_state(state);
	restore_parent_sig_handlers(state);
}

int	fork_and_collect_hd(t_hd_state *state, t_shell *shell, t_redir *redir, char **envp)
{
	state->pid = fork();
	if (state->pid < 0)
		return(-1);
	if (state->pid == 0)
	{
		set_default_sig_handling();
		close(state->fds[0]); //Child only writes.
		while (1)
		{
			if (readline_and_check_eof(state, redir) == 1)
				break ;
			if (handle_heredoc_line(state->fds[1], state->line, redir, shell->last_status, envp) == -1)
			{
				free(state->line);
				close(state->fds[1]);
				_exit(1);
			}
			free(state->line);
		}
		close(state->fds[1]);
		_exit(0);
	}
	return(0);
}

int	readline_and_check_eof(t_hd_state *state, t_redir *redir)
{
	state->line = readline("heredoc>");				// Take a line of input from the user.
	if (!state->line)								
		return (1);					
	if (ft_strcmp(state->line, redir->target) == 0)	// Check if line == EOF delimiter.
	{
		free(state->line);							// Free the delimiter line.		
		return (1);									// Break out of the readline loop.
	}
	return (0);										// Input gathered and not EOF, move on.
}

int	handle_child_status(t_hd_state *state, t_shell *shell)
{
	if (WIFSIGNALED(state->status) && WTERMSIG(state->status) == SIGINT)	// If ctrl+c
	{
		write(STDOUT_FILENO, "\n", 1);
		close(state->fds[0]);
		shell->last_status = 130;
		return (-1);
	}
	if (!WIFEXITED(state->status) || WEXITSTATUS(state->status) != 0)		// If failure
	{
		write(STDOUT_FILENO, "\n", 1);
		close(state->fds[0]);
		return (-1);
	}
	return (0);
}

int		close_pipe_err(t_hd_state *state)
{
	close(state->fds[0]);
	close(state->fds[1]);
	return (-1);
}

void	restore_terminal_state(t_hd_state *state)
{
	if (isatty(STDIN_FILENO))
		tcsetattr(STDIN_FILENO, TCSANOW, &state->tty);
}

void	init_hd_state(t_hd_state *state)
{
	ft_bzero(state, sizeof(state));
	state->fds[0] = -1;
	state->fds[1] = -1;
	state->line = NULL;
}

void	save_terminal_state(struct termios *tty)
{
	if (isatty(STDIN_FILENO))
		tcgetattr(STDIN_FILENO, tty);
}

void	free_line_close_fds(int fds[2], char *line)
{
	free(line);
	close(fds[0]);
	close(fds[1]);	
}

int	handle_heredoc_line(int	fd, char *line, t_redir *redir, int last_status, char **envp)
{
	if (redir->no_expand == 0)										// Delimiter was not quoted - Expand
	{
		if (expand_write_line(fd, line, last_status, envp) == -1)	// Rebuild the line with status and var expanded. Rebuilds the line even if there is nothing to expand :)
			return (-1);
	}
	else															// Delimiter was quoted - NO expansion
		write_line_nl(fd, line);									// Write the line as it is. 
	return (0);
}

int	expand_write_line(int fd, char *line, int last_status, char **envp)
{
	char *temp1;
	char *temp2;

	temp1 = expand_status(line, last_status);						// Rebuilds the string with status expanded
	if (!temp1)
		return (-1);
	temp2 = expand_variable(temp1, envp);							// Rebuilds temp1 with 
	if (!temp2)
	{
		free(temp1);
		return (-1);
	}
	write_line_nl(fd, temp2);
	free(temp1);
	free(temp2);
	return (0);
}

void	write_line_nl(int fd, char *line)
{
	if (!line)
		return ;
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}
