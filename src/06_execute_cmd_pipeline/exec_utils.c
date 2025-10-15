/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:26:55 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/15 14:06:27 by wheino           ###   ########.fr       */
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

void	direct_exec(char **argv, t_shell *shell, pid_t *child_pids)
{
	int	err;

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
