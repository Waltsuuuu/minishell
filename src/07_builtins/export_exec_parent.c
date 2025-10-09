/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_exec_parent.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 09:24:57 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 09:41:49 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Executes "export" as a single builtin in the parent shell.
 * Honors possible redirections; prints list when no arguments.
 *
 * @param cmd   command (argv/argc/redirs)
 * @param shell shell state (env_head, last_status)
 * @return exit status (0..255)
 */
int	exec_export_in_parent(t_command *cmd, t_shell *shell)
{
	int	saved[2];

	if (!cmd || !shell)
		return (1);
	if (apply_redirs_in_parent(cmd, saved) != 0)
	{
		return (1);
	}
	if (!cmd->argv[1] || (cmd->argv[1][0] == '\0'))
	{
		env_sort_and_print(shell);
		restore_stdio(saved);
		shell->last_status = 0;
		return (0);
	}
	shell->last_status = builtin_export(cmd->argv, shell);
	restore_stdio(saved);
	return (shell->last_status);
}

