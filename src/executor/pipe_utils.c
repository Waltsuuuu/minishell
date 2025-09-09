#include "minishell.h"

/**
 * Allocates and creates the N-1 pipes required for a pipeline of N commands.
 * Each pipe is an int[2] (read end [0], write end [1]).
 * On failure, closes already-created pipes and frees memory.
 *
 * @param cmd_count total number of commands in the pipeline
 * @return pointer to an array of (cmd_count - 1) pipe pairs, or NULL if none/failed
 *
 * @note Caller owns the returned array and must later call close_all_pipes()
 *       and free() the array.
 */
int	(*allocate_pipes(int cmd_count))[2]
{
	int	(*pipe_pairs)[2];
	int	pipe_index;

	if (cmd_count <= 1)
		return (NULL);
	pipe_pairs = (int (*)[2])malloc(sizeof(int[2]) * (cmd_count - 1)); // pipecount = cmd_count - 1
	if (!pipe_pairs)
		return (NULL);
	pipe_index = 0;
	while (pipe_index < cmd_count - 1)
	{
		if (pipe(pipe_pairs[pipe_index]) < 0)
		{
			while (pipe_index > 0)
			{
				pipe_index--;
				close(pipe_pairs[pipe_index][0]);
				close(pipe_pairs[pipe_index][1]);
			}
			free(pipe_pairs);
			return (NULL);
		}
		pipe_index++;
	}
	return (pipe_pairs);
}
/**
 * Closes all file descriptors for the given array of pipe pairs.
 *
 * @param pipe_pairs array of pipe pairs returned by allocate_pipes()
 * @param cmd_count total number of commands (determines number of pipes as N-1)
 */
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

/**
 * Computes the desired stdin/stdout FDs for a command in a pipeline.
 * For command i in [0..N-1]:
 *  - in_fd  = read end of pipe (i-1) if i > 0, else -1
 *  - out_fd = write end of pipe i     if i < N-1, else -1
 *
 * @param cmd_index zero-based index of the current command
 * @param cmd_count total number of commands in the pipeline
 * @param pipe_pairs array of pipe pairs (size N-1), from allocate_pipes()
 * @param in_fd output: desired FD to dup2() to STDIN, or -1 if none
 * @param out_fd output: desired FD to dup2() to STDOUT, or -1 if none
 */
void	compute_cmd_fds(int cmd_index, int cmd_count, int (*pipe_pairs)[2],
			int *in_fd, int *out_fd)
{
	*in_fd = -1; //initialize as -1, works as a flag that we havent touched fds.
	*out_fd = -1;
	if (cmd_count <= 1) 
		return ;
	if (cmd_index >= 1) 
		*in_fd = pipe_pairs[cmd_index - 1][0];
	if (cmd_index < cmd_count - 1)
		*out_fd = pipe_pairs[cmd_index][1];
}

/**
 * In the parent process, closes the pipe ends that are not needed anymore
 * after forking the child for command cmd_index.
 *
 * @param cmd_index zero-based index of the command just spawned
 * @param cmd_count total number of commands in the pipeline
 * @param pipe_pairs array of pipe pairs (size N-1), from allocate_pipes()
 *
 * @note Typically called once per spawned child in the parent to prevent FD leaks.
 */
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