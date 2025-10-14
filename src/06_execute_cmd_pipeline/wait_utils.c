/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 18:20:15 by wheino            #+#    #+#             */
/*   Updated: 2025/10/08 18:22:59 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	wait_all_and_last_status( int child_count,
			pid_t last_child_pid)
{
	int		index;
	int		raw_status;
	int		last_status;
	pid_t	pid;

	index = 0;
	last_status = 0;
	while (index < child_count)
	{
		pid = waitpid(-1, &raw_status, 0);
		if (pid == -1)
		{
			if (errno == EINTR)
				continue ;
			if (errno == ECHILD)
				break ;
			return (1);
		}
		if (pid == last_child_pid)
			last_status = normalize_raw_status(raw_status);
		index++;
	}
	return (last_status);
}
