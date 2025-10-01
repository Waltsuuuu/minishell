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
	// int		(*pipe_pairs)[2]; //Osoitin taulukko pareista ns putket
	// pid_t	*child_pids;
	int		cmd_index;
	int		pipe_in;
	int		pipe_out;

	pipeline->pipe_pairs = allocate_pipes(pipeline->n_cmds);
	pipeline->child_pids = (pid_t *)malloc(sizeof(pid_t) * pipeline->n_cmds);
	if (!pipeline->child_pids || (pipeline->n_cmds > 1 && !pipeline->pipe_pairs))
		return (free(pipeline->child_pids), close_all_pipes(pipeline->pipe_pairs, pipeline->n_cmds),
			free(pipeline->pipe_pairs), 1);
	cmd_index = 0;
	while (cmd_index < pipeline->n_cmds)
	{
		compute_cmd_fds(cmd_index, pipeline->n_cmds, pipeline->pipe_pairs, &pipe_in, &pipe_out);
		pipeline->child_pids[cmd_index] = spawn_cmd(&pipeline->cmds[cmd_index],					
				envp, pipe_in, pipe_out, shell, pipeline->child_pids, pipeline->pipe_pairs);
		close_parent_unused_ends(cmd_index, pipeline->n_cmds, pipeline->pipe_pairs);
		cmd_index++;
	}
	close_all_pipes(pipeline->pipe_pairs, pipeline->n_cmds);
	free(pipeline->pipe_pairs);
	shell->last_status = wait_all_and_last_status(pipeline->child_pids, pipeline->n_cmds,
			pipeline->child_pids[pipeline->n_cmds - 1]);
	free(pipeline->child_pids);
	free_allocs(shell);
	return (shell->last_status);
}

