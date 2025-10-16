/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:23:50 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/16 12:39:05 by wheino           ###   ########.fr       */
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
		cleanup(shell);
		_exit(127);
	}
	else if (saved_errno == EACCES)
	{
		ft_putstr_fd("No permission\n", STDERR_FILENO);
		cleanup(shell);
		_exit(126);
	}
	else
	{
		ft_putstr_fd("Is a directory\n", STDERR_FILENO);
		cleanup(shell);
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

void	env_path_execve_error_and_exit(t_shell *shell, char **path_directories, int err)
{
	if (err == EACCES)
	{
		ft_putstr_fd(": No permission\n", STDERR_FILENO);
		clean(path_directories, shell, shell->pipeline.child_pids);
		_exit(126);
	}
	else if (err == EISDIR)
	{
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		clean(path_directories, shell, shell->pipeline.child_pids);
		_exit(126);
	}
	else if (err == ENOEXEC)
	{
		ft_putstr_fd(": exec format error\n", STDERR_FILENO);
		clean(path_directories, shell, shell->pipeline.child_pids);
		_exit(126);
	}
	else
	{
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		clean(path_directories, shell, shell->pipeline.child_pids);
		_exit(127);
	}
}
