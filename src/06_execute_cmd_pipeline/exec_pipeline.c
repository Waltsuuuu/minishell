/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:16:43 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/07 17:37:05 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	alloc_child_pids(t_pipeline *pl)
{
	pl->child_pids = (pid_t *)malloc(sizeof(pid_t) * pl->n_cmds);
	if (!pl->child_pids)
		return (0);
	ft_bzero(pl->child_pids, sizeof(pid_t) * pl->n_cmds);
	return (1);
}

static void	init_parent_pipe_pair(t_shell *shell)
{
	shell->pl.pipe_pair[0] = -1;
	shell->pl.pipe_pair[1] = -1;
}

static void	close_if_nonneg(int fd)
{
	if (fd >= 0 && fd > 2)
		close (fd);
}

static int	on_open_pipe_error(pid_t *pids, t_exec *exec)
{
	close_if_nonneg(exec->previous_read);
	kill_and_reap_children(pids, exec->cmd_index);
	free(pids);
	restore_parent_sig_handlers(&exec->old_quit, &exec->old_int);
	return (1);
}

static int	on_spawn_error(t_pipeline *pl, t_exec *exec)
{
	close_if_nonneg(exec->next_write);
	close_if_nonneg(exec->next_read);
	close_if_nonneg(exec->previous_read);
	kill_and_reap_children(pl->child_pids, exec->cmd_index);
	free(pl->child_pids);
	restore_parent_sig_handlers(&exec->old_quit, &exec->old_int);
	return (1);
}

static void	after_successful_spawn_parent_close_step(t_exec *exec)
{
	close_if_nonneg(exec->previous_read);
	close_if_nonneg(exec->next_write);
	exec->previous_read = exec->next_read;
}

static int	finalize_and_return(t_pipeline *pl, t_shell *shell, t_exec *exec)
{
	if (exec->previous_read >= 0)
		close(exec->previous_read);
	shell->last_status = wait_all_and_last_status(pl->n_cmds,
			pl->child_pids[pl->n_cmds - 1]);
	restore_parent_sig_handlers(&exec->old_quit, &exec->old_int);
	restore_terminal_state(&exec->tty);
	if (shell->last_status == 130)
		write(1, "\n", 1);
	free(pl->child_pids);
	free_allocs(shell);
	return (shell->last_status);
}

int	exec_pipeline(t_pipeline *pl, t_shell *shell)
{
	t_exec	exec;

	exec.cmd_index = 0;
	if (alloc_child_pids(pl) == 0)
		return (1);
	exec.previous_read = -1;
	init_parent_pipe_pair(shell);
	save_terminal_state(&exec.tty);
	ignore_parent_sig_handlers(&exec.ign, &exec.old_quit, &exec.old_int);
	while (exec.cmd_index < pl->n_cmds)
	{
		if (open_next_pipe_if_needed(shell, &exec) < 0)
			return (on_open_pipe_error(pl->child_pids, &exec));
		pl->child_pids[exec.cmd_index] = spawn_cmd(&pl->cmds[exec.cmd_index],
				exec.previous_read, exec.next_write, shell);
		if (pl->child_pids[exec.cmd_index] < 0)
			return (on_spawn_error(pl, &exec));
		after_successful_spawn_parent_close_step(&exec);
		exec.cmd_index++;
	}
	shell->last_status = finalize_and_return(pl, shell, &exec);
	return (shell->last_status);
}
