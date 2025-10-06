#include "minishell.h"

// Collects the users input via readline() and checks if the line == delimiter.
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

// Checks if line has to be expanded before writing to the pipe.
int	handle_heredoc_line(t_hd_state *state, int fd, char *line, t_redir *redir, int last_status, char **envp)
{
	if (redir->no_expand == 0)										// Delimiter was not quoted - Expand
	{
		if (expand_write_line(state, fd, line, last_status, envp) == -1)	// Rebuild the line with status and var expanded. Rebuilds the line even if there is nothing to expand :)
			return (-1);
	}
	else															// Delimiter was quoted - NO expansion
		write_line_nl(fd, line);									// Write the line as it is. 
	return (0);
}

int	expand_write_line(t_hd_state *state, int fd, char *line, int last_status, char **envp)
{
	char *temp1;	// Line with status expanded.
	char *temp2;	// Line with status and var expaneded.

	temp1 = expand_status(line, last_status);	// Rebuilds the inpit line with status expanded.
	if (!temp1)
		return (-1);
	temp2 = expand_variable(temp1, envp);		// Rebuilds temp1 with var exapnded.
	if (!temp2)
	{
		free(temp1);
		return (-1);
	}
	state->pipe_bytes_written += ft_strlen(temp2);
	if (state->pipe_bytes_written > PIPE_BUF_MAX)
	{
		free(temp1);
		free(temp2);
		ft_putstr_fd("minishell: Error! Heredoc max size 63KiB\n", 2);
		return (-1);
	}
	write_line_nl(fd, temp2);					// Write the expaneded line to the pipe.
	free(temp1);
	free(temp2);
	return (0);
}

// Writes the input line, followed by a newline, to the pipe.
void	write_line_nl(int fd, char *line)
{
	if (!line)
		return ;
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}
