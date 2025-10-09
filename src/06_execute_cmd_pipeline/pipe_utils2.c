/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:11:41 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 09:13:30 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	child_close_all_pipes(t_shell *shell)
{
	if (!shell)
		return ;
	if (shell->pipeline.pipe_pair[0] >= 3)
		close (shell->pipeline.pipe_pair[0]);
	else if (shell->pipeline.pipe_pair[1] >= 3)
		close (shell->pipeline.pipe_pair[1]);
}

void	set_child_fds_from_pipes(int *final_in, int *final_out,
									int pipe_in, int pipe_out)
{
	*final_in = STDIN_FILENO;
	if (pipe_in >= 0)
		*final_in = pipe_in;
	*final_out = STDOUT_FILENO;
	if (pipe_out >= 0)
		*final_out = pipe_out;
}

void	replug_child_stdin(int final_in)
{
	if (final_in != STDIN_FILENO)
	{
		if (dup2(final_in, STDIN_FILENO) < 0)
			_exit(1);
		close(final_in);
	}
}

void	replug_child_stdout(int final_out)
{
	if (final_out != STDOUT_FILENO)
	{
		if (dup2(final_out, STDOUT_FILENO) < 0)
			_exit(1);
		close(final_out);
	}
}

void	child_finalize_pipes(t_shell *shell)
{
	child_close_all_pipes(shell);
}