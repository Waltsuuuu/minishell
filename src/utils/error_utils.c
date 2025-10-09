/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:23:50 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 09:24:58 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execve_error_and_exit(t_shell *shell, char **argv,
									pid_t *child_pids, int saved_errno)
{
	if (child_pids)
		free(child_pids);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(argv[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (saved_errno == ENOENT || saved_errno == ENOTDIR)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		clean_env(&shell->env_head);
		free_split(&shell->env_arr);
		free_allocs(shell);
		_exit(127);
	}
	else
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		clean_env(&shell->env_head);
		free_split(&shell->env_arr);
		free_allocs(shell);
		_exit(126);
	}
}

void	print_redir_error(t_redir *redir)
{
	if (redir && redir->target)
		perror(redir->target);
	else
		perror("redir");
}