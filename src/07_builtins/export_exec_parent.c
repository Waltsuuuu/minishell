#include "minishell.h"

/**
 * replug_stdio_pair - Redirect STDIN/STDOUT to given FDs and close originals.
 *
 * Duplicates in_fd → STDIN and out_fd → STDOUT (if different), restoring
 * previously saved stdio on error. On success, closes the original fds
 * (when not 0/1) and leaves redirection active.
 *
 * @param in_fd  New stdin FD (or STDIN_FILENO / <0 to skip).
 * @param out_fd New stdout FD (or STDOUT_FILENO / <0 to skip).
 * @param saved  Pair of saved stdio FDs from save_stdio(saved).
 * @return 0 on success; -1 on error (stdio restored).
 */
static int	replug_stdio_pair(int in_fd, int out_fd, int saved[2])
{
	if (in_fd >= 0 && in_fd != STDIN_FILENO
		&& dup2(in_fd, STDIN_FILENO) < 0)
	{
		restore_stdio(saved);
		if (out_fd >= 0 && out_fd != STDOUT_FILENO)
			close(out_fd);
		close(in_fd);
		return (-1);
	}
	if (in_fd >= 0 && in_fd != STDIN_FILENO)
		close(in_fd);
	if (out_fd >= 0 && out_fd != STDOUT_FILENO
		&& dup2(out_fd, STDOUT_FILENO) < 0)
	{
		restore_stdio(saved);
		close(out_fd);
		return (-1);
	}
	if (out_fd >= 0 && out_fd != STDOUT_FILENO)
		close(out_fd);
	return (0);
}

/**
 * Executes "export" as a single builtin in the parent shell.
 * Honors possible redirections; prints list when no arguments.
 *
 * @param cmd   command (argv/argc/redirs)
 * @param shell shell state (env_head, last_status)
 * @return exit status (0..255)
 */
int	exec_export_in_parent(t_command *cmd, t_shell *shell)
{
	int	saved[2];

	if (!cmd || !shell)
		return (1);
	if (apply_redirs_in_parent(cmd, saved) != 0)
		return (1);
	if (!cmd->argv[1] || (cmd->argv[1][0] == '\0'))
	{
		env_sort_and_print(shell);
		restore_stdio(saved);
		shell->last_status = 0;
		return (0);
	}
	shell->last_status = builtin_export(cmd->argv, shell);
	restore_stdio(saved);
	return (shell->last_status);
}

/**
 * Applies <, >, >> redirections for a single builtin in the parent.
 * On error, restores stdio and returns -1.
 *
 * @param cmd   command with redirections
 * @param saved stdio backups (modified by save/restore)
 * @return 0 on success, -1 on failure
 */
int	apply_redirs_in_parent(t_command *cmd, int saved[2])
{
	t_list	*node;
	t_redir	*r;
	int		in_fd;
	int		out_fd;

	if (save_stdio(saved) < 0)
		return (-1);
	in_fd = STDIN_FILENO;
	out_fd = STDOUT_FILENO;
	node = cmd->redirs;
	while (node)
	{
		r = (t_redir *)node->content;
		if (r && r->type == REDIR_OUT && apply_redir_out(r, &out_fd) < 0)
			return (restore_stdio(saved), -1);
		if (r && r->type == REDIR_APPEND && apply_redir_append(r, &out_fd) < 0)
			return (restore_stdio(saved), -1);
		if (r && r->type == REDIR_IN && apply_redir_in(r, &in_fd) < 0)
			return (restore_stdio(saved), -1);
		node = node->next;
	}
	if (replug_stdio_pair(in_fd, out_fd, saved) < 0)
		return (-1);
	return (0);
}

/**
 * Saves current STDIN/STDOUT FDs into saved[0]/saved[1].
 * Used before temporary redirections in the parent process.
 *
 * @param saved array of two ints to store backups
 * @return 0 on success, -1 on error (perror printed)
 */
int	save_stdio(int saved[2])
{
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	if (saved[0] < 0 || saved[1] < 0)
	{
		perror("dup");
		return (-1);
	}
	return (0);
}

/**
 * Restores STDIN/STDOUT from saved[] backups and closes the backups.
 *
 * @param saved array from save_stdio
 */
void	restore_stdio(int saved[2])
{
	if (saved[0] >= 0)
	{
		dup2(saved[0], STDIN_FILENO);
		close(saved[0]);
	}
	if (saved[1] >= 0)
	{
		dup2(saved[1], STDOUT_FILENO);
		close(saved[1]);
	}
}
