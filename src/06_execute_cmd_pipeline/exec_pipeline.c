#include "minishell.h"

/**
 * Executes a p of commands.
 * Spawns one child per command, wires pipes between them, closes unused FDs
 * in the parent, and waits for all children. Returns the exit status of the
 * last command in the p (bash-compatible).
 *
 * @param argv_per_cmd array of argv arrays; argv_per_cmd[i][0] is command i
 * @param p->n_cmds number of commands in the p (N)
 * @param envp environment vector passed to each execve
 * @return normalized exit status (0–255) of the last command; 1 on setup error
 *
 * @note On failure to allocate/create pipes or PIDs, cleans up and returns 1.
 *       Caller owns argv_per_cmd contents; this function does not free them.
 */
int	exec_pipeline(t_pipeline *pipeline, t_shell *shell)
{
	int		cmd_index;
	int		previous_read;
	int		next_read;
	int		next_write;

	pipeline->child_pids = (pid_t *)malloc(sizeof(pid_t) * pipeline->n_cmds);
	if (!pipeline->child_pids)
	{
		return (1);
	}
	ft_bzero(pipeline->child_pids, sizeof(pid_t) * pipeline->n_cmds);
	previous_read = -1;
	cmd_index = 0;
	while (cmd_index < pipeline->n_cmds)
	{
		if (open_next_pipe_if_needed(cmd_index, pipeline->n_cmds, &next_read, &next_write) < 0)
		{
			if (previous_read >= 0)
				close(previous_read);
			kill_and_reap_children(pipeline->child_pids, cmd_index);
			free (pipeline->child_pids);
			return (1); //TODO ERROR MSG
		}
		pipeline->child_pids[cmd_index] = spawn_cmd(&pipeline->cmds[cmd_index],					
				previous_read, next_write, shell);
		if (pipeline->child_pids[cmd_index] < 0)
		{
			if (next_write >= 0)
				close(next_write);
			if (next_read  >= 0)
				close(next_read);
			if (previous_read  >= 0)
				close(previous_read);
			kill_and_reap_children(pipeline->child_pids, cmd_index);
			free (pipeline->child_pids);
			return (1); //TODO ERROR MSG
		}
		if (previous_read >= 0)
			close(previous_read);
		if (next_write >= 0)
			close (next_write);
		previous_read = next_read;
		cmd_index++;
	}
	if (previous_read >= 0)
		close(previous_read);
	shell->last_status = wait_all_and_last_status(pipeline->child_pids, pipeline->n_cmds,
			pipeline->child_pids[pipeline->n_cmds - 1]);
	free(pipeline->child_pids);
	free_allocs(shell);
	return (shell->last_status);
}


