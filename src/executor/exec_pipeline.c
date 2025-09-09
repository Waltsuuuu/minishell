#include "minishell.h"

/**
 * Executes a pipeline of commands.
 * Spawns one child per command, wires pipes between them, closes unused FDs
 * in the parent, and waits for all children. Returns the exit status of the
 * last command in the pipeline (bash-compatible).
 *
 * @param argv_per_cmd array of argv arrays; argv_per_cmd[i][0] is command i
 * @param cmd_count number of commands in the pipeline (N)
 * @param envp environment vector passed to each execve
 * @return normalized exit status (0–255) of the last command; 1 on setup error
 *
 * @note On failure to allocate/create pipes or PIDs, cleans up and returns 1.
 *       Caller owns argv_per_cmd contents; this function does not free them.
 */
int	exec_pipeline(char ***argv_per_cmd, int cmd_count, char **envp, t_pipeline *pipeline)
{
	int		(*pipe_pairs)[2]; //Osoitin taulukko pareista ns putket
	pid_t	*child_pids;
	int		cmd_index;
	int		in_fd;
	int		out_fd;
	int		last_status;

	pipe_pairs = allocate_pipes(cmd_count);
	child_pids = (pid_t *)malloc(sizeof(pid_t) * cmd_count);
	if (!child_pids || (cmd_count > 1 && !pipe_pairs))
		return (free(child_pids), close_all_pipes(pipe_pairs, cmd_count),
			free(pipe_pairs), 1);
	cmd_index = 0;
	while (cmd_index < cmd_count)
	{
		compute_cmd_fds(cmd_index, cmd_count, pipe_pairs, &in_fd, &out_fd);
		child_pids[cmd_index] = spawn_cmd(argv_per_cmd[cmd_index],
				envp, in_fd, out_fd);
		close_parent_unused_ends(cmd_index, cmd_count, pipe_pairs);
		cmd_index++;
	}
	close_all_pipes(pipe_pairs, cmd_count);
	free(pipe_pairs);
	last_status = wait_all_and_last_status(child_pids, cmd_count,
			child_pids[cmd_count - 1]);
	free(child_pids);
	return (last_status);
}

