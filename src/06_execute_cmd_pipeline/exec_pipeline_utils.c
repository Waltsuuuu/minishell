/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 08:50:25 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 10:46:35 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	alloc_child_pids(t_pipeline *pl)
{
	pl->child_pids = (pid_t *)malloc(sizeof(pid_t) * pl->n_cmds);
	if (!pl->child_pids)
		return (0);
	ft_bzero(pl->child_pids, sizeof(pid_t) * pl->n_cmds);
	return (1);
}

void	init_parent_pipe_pair(t_shell *shell)
{
	shell->pipeline.pipe_pair[0] = -1;
	shell->pipeline.pipe_pair[1] = -1;
}

void	close_if_nonneg(int fd)
{
	if (fd >= 0 && fd > 2)
		close (fd);
}

int	on_open_pipe_error(pid_t *pids, t_exec *exec)
{
	close_if_nonneg(exec->previous_read);
	kill_and_reap_children(pids, exec->cmd_index);
	free(pids);
	restore_parent_sig_handlers(&exec->old_quit, &exec->old_int);
	return (1);
}

int	on_spawn_error(t_pipeline *pl, t_exec *exec)
{
	close_if_nonneg(exec->next_write);
	close_if_nonneg(exec->next_read);
	close_if_nonneg(exec->previous_read);
	kill_and_reap_children(pl->child_pids, exec->cmd_index);
	free(pl->child_pids);
	restore_parent_sig_handlers(&exec->old_quit, &exec->old_int);
	return (1);
}
