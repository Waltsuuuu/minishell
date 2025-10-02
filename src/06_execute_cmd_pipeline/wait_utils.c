/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:57:58 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/02 15:15:52 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Waits for a specific child process to finish.
 *
 * @param target_pid the PID of the child process to wait for
 * @param out_raw_status pointer to store the raw wait status
 * @return 0 on success (child finished or no child), -1 on unexpected error
 */
int	wait_for_pid_once(pid_t target_pid, int *out_raw_status)
{
	int	raw_status;
	int	wait_result;

	if (target_pid <= 0)
		return (0);
	while (1)
	{
		wait_result = waitpid(target_pid, &raw_status, 0);
		if (wait_result == target_pid)
		{
			*out_raw_status = raw_status;
			return (0);
		}
		if (wait_result == -1 && errno == EINTR)
			continue ;
		if (wait_result == -1 && errno == ECHILD)
			return (0);
		return (-1);
	}
}

/**
 * Normalizes a raw wait status into a shell-style exit code.
 *
 * @param raw_status the raw status returned by waitpid
 * @return the exit code (0–255) or signal-based code (128 + signal number)
 */
static int	normalize_raw_status(int raw_status)
{
	if (WIFEXITED(raw_status))
		return (WEXITSTATUS(raw_status));
	if (WIFSIGNALED(raw_status))
		return (128 + WTERMSIG(raw_status));
	return (1);
}

/**
 * Waits for all given child processes and returns 
 * the exit status of the last one.
 *
 * @param child_pids array of child process IDs
 * @param child_count number of children in the array
 * @param last_child_pid the PID of the last command in a pipeline
 * @return normalized exit status of the last child process
 */
int	wait_all_and_last_status(pid_t *child_pids, int child_count,
			pid_t last_child_pid)
{
	int	index;
	int	raw_status;
	int	last_status;

	index = 0;
	last_status = 0;
	while (index < child_count)
	{
		if (wait_for_pid_once(child_pids[index], &raw_status) == 0)
			if (child_pids[index] == last_child_pid)
				last_status = normalize_raw_status(raw_status);
		index++;
	}
	return (last_status);
}
