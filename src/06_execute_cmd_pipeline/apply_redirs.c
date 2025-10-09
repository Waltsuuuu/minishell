/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirs.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:05:08 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 10:47:00 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redir_in(const t_redir *redir, int *final_in)
{
	int	fd;

	if (!redir || !redir->target || !final_in)
		return (-1);
	fd = open(redir->target, O_RDONLY);
	if (fd < 0)
		return (-1);
	if (*final_in != STDIN_FILENO && *final_in >= 0 && *final_in != fd)
		close(*final_in);
	*final_in = fd;
	return (0);
}

int	apply_redir_append(const t_redir *redir, int *final_out)
{
	int	fd;

	if (!redir || !redir->target || !final_out)
		return (-1);
	fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd < 0)
		return (-1);
	if (*final_out != STDOUT_FILENO && *final_out >= 0 && *final_out != fd)
		close(*final_out);
	*final_out = fd;
	return (0);
}

int	apply_redir_heredoc(const t_redir *redir, int *final_in)
{
	int	fd;

	if (!redir || !redir->target || !final_in)
		return (-1);
	fd = redir->hd_fd;
	if (fd < 0)
		return (-1);
	if (*final_in != STDIN_FILENO && *final_in >= 0 && *final_in != fd)
		close(*final_in);
	*final_in = fd;
	return (0);
}

int	apply_redir_out(const t_redir *redir, int *final_out)
{
	int	fd;

	if (!redir || !redir->target || !final_out)
		return (-1);
	fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd < 0)
		return (-1);
	if (*final_out != STDOUT_FILENO && *final_out >= 0 && *final_out != fd)
		close(*final_out);
	*final_out = fd;
	return (0);
}
