#include "minishell.h"

// Loops through commands
int	collect_heredocs(t_pipeline *pipeline)
{
	int	cmd_i;

	if (!pipeline || !pipeline->cmds || pipeline->n_cmds <= 0)
		return (-1);
	cmd_i = 0;
	while (cmd_i < pipeline->n_cmds)								// Loop over all commands
	{
		if (collect_cmd_heredocs(&pipeline->cmds[cmd_i]) == -1)		// Loop over the redirects of the command and check for heredocs.
			return (-1);
		cmd_i++;													// Move to next command.
	}
	return (0);
}
// Loops all redirs in a command (looking for heredoc redir)
int	collect_cmd_heredocs(t_command *cmd)
{
	t_list 	*current;
	t_redir	*redir;

	current = cmd->redirs;
	while (current)										// Loop over the redirect linked list of this specific command
	{
		redir = (t_redir *)current->content;
		if (redir && redir->type == REDIR_HEREDOC)		// If the redir type is heredoc, go collect the body
		{
			if (collect_heredoc_body(redir) == -1)		// 		-Collect heredoc body.
				return (-1);
		}
		current = current->next;						// Move to next redirect.
	}
	return (0);											
}
// Collects the heredoc body for one heredoc.
int	collect_heredoc_body(t_redir *redir)
{
	int		fds[2];
	char	*line;

	if (pipe(fds) == -1)
		return (-1);
	while (1)
	{
		line = readline("heredoc>");				// Take a line of input from the user.
		if (!line)
			break ;
		if (ft_strcmp(line, redir->target) == 0)   // Check if line == EOF delimiter.
 		{
			free(line);		// Free the delimiter line.		
			break ;			// Break out of the readline loop.
		}
		write(fds[1], line, ft_strlen(line));
		write(fds[1], "\n", 1);
		free(line);
	}
	close(fds[1]);
	redir->hd_fd = fds[0];
	return (0);
}