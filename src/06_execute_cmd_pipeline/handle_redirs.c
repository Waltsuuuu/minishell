/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redirs.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:06:48 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 09:07:11 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_redir_error(t_shell *shell, t_redir *redir)
{
	print_redir_error(redir);
	free_allocs(shell);
	free(shell->pipeline.child_pids);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
	_exit(1);
}

void	handle_redir_out(t_redir *r, int *final_out, t_shell *shell)
{
	if (r && r->type == REDIR_OUT)
	{
		if (apply_redir_out(r, final_out) < 0)
			handle_redir_error(shell, r);
	}
}

void	handle_redir_append(t_redir *r, int *final_out, t_shell *shell)
{
	if (r && r->type == REDIR_APPEND)
	{
		if (apply_redir_append(r, final_out) < 0)
			handle_redir_error(shell, r);
	}
}

void	handle_redir_in(t_redir *r, int *final_in, t_shell *shell)
{
	if (r && r->type == REDIR_IN)
	{
		if (apply_redir_in(r, final_in) < 0)
			handle_redir_error(shell, r);
	}
}

void	handle_redir_heredoc(t_redir *r, int *final_in, t_shell *shell)
{
	if (r && r->type == REDIR_HEREDOC)
	{
		if (apply_redir_heredoc(r, final_in) < 0)
			handle_redir_error(shell, r);
	}
}