/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:26:55 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/16 18:43:36 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	has_slash(char *input)
{
	if (!input)
		return (0);
	while (*input)
		if (*input++ == '/')
			return (1);
	return (0);
}

/**
 * @brief Checks if the current path leads to a directory.
 * @return 0 if not a dirctory.
 */
int	is_dir(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
		return (0);
	return (S_ISDIR(st.st_mode));
}

void	is_dir_exit(t_shell *shell, char **argv, pid_t *child_pids)
{
	if (child_pids)
		free(child_pids);
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(argv[0], STDERR_FILENO);
	ft_putstr_fd(": Is a directory\n", STDERR_FILENO);
	cleanup(shell);
	_exit(126);
}

void	direct_exec(char **argv, t_shell *shell, pid_t *child_pids)
{
	int	err;

	if (is_dir(argv[0]))
		is_dir_exit(shell, argv, shell->pipeline.child_pids);
	execve(argv[0], argv, shell->env_arr);
	err = errno;
	execve_error_and_exit(shell, argv, child_pids, err);
}

void	clean(char **directories, t_shell *shell, pid_t *child_pids)
{
	free_split(&directories);
	free_shell(shell);
	if (child_pids)
		free(child_pids);
}
