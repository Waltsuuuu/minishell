/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pipeline.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/02 09:27:12 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/02 09:54:16 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Executes a p of commands.
 * Spawns one child per command, wires pipes between them, closes unused FDs
 * in the parent, and waits for all children. Returns the exit status of the
 * last command in the p (bash-compatible).
 *
 * @param argv_per_cmd array of argv arrays; argv_per_cmd[i][0] is command i
 * @param p->n_cmds number of commands in the p (N)
 * @return normalized exit status (0–255) of the last command; 1 on setup error
 *
 * @note On failure to allocate/create pipes or PIDs, cleans up and returns 1.
 *       Caller owns argv_per_cmd contents; this function does not free them.
 */
int	exec_pipeline(t_pipeline *p, t_shell *shell)
{
	int		cmd_index;
	int		p_in;
	int		p_out;

	p->pipe_pairs = allocate_pipes(p->n_cmds);
	p->child_pids = (pid_t *)malloc(sizeof(pid_t) * p->n_cmds);
	if (!p->child_pids || (p->n_cmds > 1 && !p->pipe_pairs))
		return (free(p->child_pids), close_all_pipes(p->pipe_pairs, p->n_cmds),
			free(p->pipe_pairs), 1);
	cmd_index = 0;
	while (cmd_index < p->n_cmds)
	{
		compute_cmd_fds(cmd_index, p, &p_in, &p_out);
		p->child_pids[cmd_index] = spawn_cmd(&p->cmds[cmd_index],
				p_in, p_out, shell, p->child_pids, p->pipe_pairs);
		close_parent_unused_ends(cmd_index, p->n_cmds, p->pipe_pairs);
		cmd_index++;
	}
	close_all_pipes(p->pipe_pairs, p->n_cmds);
	free(p->pipe_pairs);
	shell->last_status = wait_all_and_last_status(p->child_pids, p->n_cmds,
			p->child_pids[p->n_cmds - 1]);
	free(p->child_pids);
	free_allocs(shell);
	return (shell->last_status);
}
