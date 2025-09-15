#include "minishell.h"

/**
 * Executes a pipeline of commands.
 * Spawns one child per command, wires pipes between them, closes unused FDs
 * in the parent, and waits for all children. Returns the exit status of the
 * last command in the pipeline (bash-compatible).
 *
 * @param argv_per_cmd array of argv arrays; argv_per_cmd[i][0] is command i
 * @param pipeline->n_cmds number of commands in the pipeline (N)
 * @param envp environment vector passed to each execve
 * @return normalized exit status (0–255) of the last command; 1 on setup error
 *
 * @note On failure to allocate/create pipes or PIDs, cleans up and returns 1.
 *       Caller owns argv_per_cmd contents; this function does not free them.
 */
int	exec_pipeline(char **envp, t_pipeline *pipeline, t_shell *shell)
{
	int		(*pipe_pairs)[2]; //Osoitin taulukko pareista ns putket
	pid_t	*child_pids;
	int		cmd_index;
	int		pipe_in;
	int		pipe_out;
	int		last_status;

	pipe_pairs = allocate_pipes(pipeline->n_cmds);
	child_pids = (pid_t *)malloc(sizeof(pid_t) * pipeline->n_cmds);
	if (!child_pids || (pipeline->n_cmds > 1 && !pipe_pairs))
		return (free(child_pids), close_all_pipes(pipe_pairs, pipeline->n_cmds),
			free(pipe_pairs), 1);
	cmd_index = 0;
	while (cmd_index < pipeline->n_cmds)
	{
		compute_cmd_fds(cmd_index, pipeline->n_cmds, pipe_pairs, &pipe_in, &pipe_out);
		child_pids[cmd_index] = spawn_cmd(&pipeline->cmds[cmd_index],					
				envp, pipe_in, pipe_out, shell, child_pids, pipe_pairs);
		close_parent_unused_ends(cmd_index, pipeline->n_cmds, pipe_pairs);
		cmd_index++;
	}
	close_all_pipes(pipe_pairs, pipeline->n_cmds);
	free(pipe_pairs);
	last_status = wait_all_and_last_status(child_pids, pipeline->n_cmds,
			child_pids[pipeline->n_cmds - 1]);
	free(child_pids);
	free_allocs(shell);
	return (last_status);
}

