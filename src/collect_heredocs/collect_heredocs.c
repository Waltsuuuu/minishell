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
	int		fds[2];
	char	*line;

	if (pipe(fds) == -1)
		return (-1);
	setup_sig_handlers_for_heredoc();
	while (1)
	{
		rl_done = 0;
		line = readline("heredoc>");				// Take a line of input from the user.
		if (g_signal == SIGINT)
		{
			if (line)
				free(line);
			shell->last_status = 130;
			close(fds[0]);
			close(fds[1]); 
			setup_signal_handlers_for_prompt();
			g_signal = 0;
			return(-1);
		}
		if (!line)
		{
			break ;
		}
		if (ft_strcmp(line, redir->target) == 0)   // Check if line == EOF delimiter.
 		{
			free(line);		// Free the delimiter line.		
			break ;			// Break out of the readline loop.
		}
		if (handle_heredoc_line(fds[1], line, redir, shell->last_status, envp) == -1)
		{
			free_line_close_fds(fds, line);
			return (-1);
		}
		free(line);
	}
	close(fds[1]);
	redir->hd_fd = fds[0];
	setup_signal_handlers_for_prompt();
	return (0);
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
// Same as prompt signal handler, except 
// - no rl_redisplay(), as we dont want to prompt for new input.
// - rl_done is set to true to stop input loop and make readline() return.
void	heredoc_sigint(int signum)
{
	g_signal = signum;
	write(STDOUT_FILENO, "\n", 1);
	// rl_on_new_line();
	rl_replace_line("", 0);
	rl_done = 1;
}

void	setup_sig_handlers_for_heredoc(void)
{
	struct sigaction sa;
	
	ft_bzero(&sa, sizeof(sa));			// Zero init the struct
	sigemptyset(&sa.sa_mask);			// No mask, no signals blocked
	sa.sa_flags = 0;					// No flags
	sa.sa_handler = heredoc_sigint;		// Use heredoc sig handler
	sigaction(SIGINT, &sa, NULL);		// Handle ctrl-c during heredoc body collection
	signal(SIGQUIT, SIG_IGN);			// Ignore ctrl-\ during heredoc body collection
}