/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:23:50 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 16:45:58 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	permission_err_and_exit(void)
{
	ft_putstr_fd("No permission\n", STDERR_FILENO);
	_exit(126);
}

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
	else if (saved_errno == EACCES)
		permission_err_and_exit();
	else
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		clean_env(&shell->env_head);
		free_split(&shell->env_arr);
		free_allocs(shell);
		_exit(126);
	}
}

/**
 * Prints "not a valid identifier" message for a builtin.
 *
 * @param builtin  builtin name ("export", etc.)
 * @param key      offending key string
 */
void	print_invalid_identifier(char *builtin, char *key)
{
	ft_putstr_fd(builtin, STDERR_FILENO);
	ft_putstr_fd(": `", STDERR_FILENO);
	ft_putstr_fd(key, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

void	print_redir_error(t_redir *redir)
{
	if (redir && redir->target)
		perror(redir->target);
	else
		perror("redir");
}