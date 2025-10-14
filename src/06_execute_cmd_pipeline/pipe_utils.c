/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 10:28:06 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/08 18:23:16 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//returns 0 if ok (or no next), -1 on error; sets next_r/next_w or -1/-1
int	open_next_pipe_if_needed(t_shell *s, t_exec *exec)
{
	if (exec->cmd_index < s->pipeline.n_cmds - 1)
	{
		if (pipe(s->pipeline.pipe_pair) < 0)
			return (-1);
		exec->next_read = s->pipeline.pipe_pair[0];
		exec->next_write = s->pipeline.pipe_pair[1];
		return (0);
	}
	exec->next_read = -1;
	exec->next_write = -1;
	s->pipeline.pipe_pair[0] = -1;
	s->pipeline.pipe_pair[1] = -1;
	return (0);
}

static void	send_signal_children(pid_t *child_pids, int spawned_count, int sig)
{
	int	i;

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
	int	i;
	int	status;

	i = 0;
	while (i < spawned_count)
	{
		if (child_pids[i] > 0)
		{
			while (waitpid(child_pids[i], &status, 0) == -1 && errno == EINTR)
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
	send_signal_children(child_pids, spawned_count, SIGKILL);
	reap_children(child_pids, spawned_count);
}
