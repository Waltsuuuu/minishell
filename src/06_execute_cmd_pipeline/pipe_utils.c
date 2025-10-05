#include "minishell.h"

 //returns 0 if ok (or no next), -1 on error; sets next_r/next_w or -1/-1 
int open_next_pipe_if_needed(int cmd_index, t_shell *shell, int *next_read, int *next_write)
{

	 if (cmd_index < shell->pipeline.n_cmds - 1)
    {
        if (pipe(shell->pipeline.pipe_pair) < 0)
            return (-1);
        *next_read = shell->pipeline.pipe_pair[0];
        *next_write = shell->pipeline.pipe_pair[1];
        return (0);
    }
    *next_read = -1;
    *next_write = -1;

    shell->pipeline.pipe_pair[0] = -1;
    shell->pipeline.pipe_pair[1] = -1;
    return (0);
}
/*


 * Closes all file descriptors for the given array of pipe pairs.
 *
 * @param pipe_pairs array of pipe pairs returned by allocate_pipes()
 * @param cmd_count total number of commands (determines number of pipes as N-1)
 *
void	close_all_pipes(int (*pipe_pairs)[2], int cmd_count)
{
	int	pipe_index;

	if (!pipe_pairs)
		return ;
	pipe_index = 0;
	while (pipe_index < cmd_count - 1)
	{
		close(pipe_pairs[pipe_index][0]);
		close(pipe_pairs[pipe_index][1]);
		pipe_index++;
	}
}


 * Computes the desired stdin/stdout FDs for a command in a pipeline.
 * For command i in [0..N-1]:
 *  - in_fd  = read end of pipe (i-1) if i > 0, else -1
 *  - out_fd = write end of pipe i     if i < N-1, else -1
 *
 * @param cmd_index zero-based index of the current command
 * @param pipeline pointer to pipeline struct
 * @param in_fd output: desired FD to dup2() to STDIN, or -1 if none
 * @param out_fd output: desired FD to dup2() to STDOUT, or -1 if none
 
void	compute_cmd_fds(int cmd_index, t_pipeline *pipeline,
			int *in_fd, int *out_fd)
{
	*in_fd = -1;
	*out_fd = -1;
	if (pipeline->n_cmds <= 1)
		return ;
	if (cmd_index >= 1)
		*in_fd = pipeline->pipe_pairs[cmd_index - 1][0];
	if (cmd_index < pipeline->n_cmds - 1)
		*out_fd = pipeline->pipe_pairs[cmd_index][1];
}


 * In the parent process, closes the pipe ends that are not needed anymore
 * after forking the child for command cmd_index.
 *
 * @param cmd_index zero-based index of the command just spawned
 * @param cmd_count total number of commands in the pipeline
 * @param pipe_pairs array of pipe pairs (size N-1), from allocate_pipes()
 *
 * @note Typically called once per spawned child in the parent 
 * to prevent FD leaks.
 *
void	close_parent_unused_ends(int cmd_index, int cmd_count,
			int (*pipe_pairs)[2])
{
	if (!pipe_pairs || cmd_count <= 1)
		return ;
	if (cmd_index > 0)
		close(pipe_pairs[cmd_index - 1][0]);
	if (cmd_index < cmd_count - 1)
		close(pipe_pairs[cmd_index][1]);
}
*/

static void	send_signal_children(pid_t *child_pids, int spawned_count, int sig)
{
	int i;

	i = 0;
	while (i < spawned_count)
	{
		if (child_pids[i] > 0)
			kill(child_pids[i], sig);
		i++;
	}
}

static void	reap_children(pid_t *child_pids, int spawned_count)
{
	int i;
	int st;

	i = 0;
	while (i < spawned_count)
	{
		if (child_pids[i] > 0)
		{
			while (waitpid(child_pids[i], &st, 0) == -1 && errno == EINTR)
				;
		}
		i++;
	}
}

/* Public: kill nicely, then force, then reap (blocks until reaped). */
void	kill_and_reap_children(pid_t *child_pids, int spawned_count)
{
	send_signal_children(child_pids, spawned_count, SIGTERM);
	reap_children(child_pids, spawned_count);
	/* If any survived (e.g. ignored SIGTERM), SIGKILL and reap again. */
	send_signal_children(child_pids, spawned_count, SIGKILL);
	reap_children(child_pids, spawned_count);
}