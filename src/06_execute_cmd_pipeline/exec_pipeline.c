/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:16:43 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 08:51:51 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
