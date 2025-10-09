/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:55:49 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 09:39:18 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	cleanup(t_shell *shell)
{
	free_allocs(shell);
	free_split(&shell->env_arr);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
}

static void	argument_validation(t_command *cmd, t_shell *shell)
{
	if (!shell->in_child && is_interactive(shell))
		ft_putstr_fd("exit\n", 1);
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(cmd->argv[1], 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	cleanup(shell);
	if (shell->in_child)
	{
		free(shell->pipeline.child_pids);
		_exit(2);
	}
	exit(2);
}

static void	exit_with_no_arg(t_shell *shell)
{
	int	status;

	status = (unsigned char)shell->last_status;
	if (shell->in_child)
	{
		cleanup(shell);
		free(shell->pipeline.child_pids);
		_exit(status);
	}
	if (is_interactive(shell))
		ft_putstr_fd("exit\n", 1);
	cleanup(shell);
	exit(status);
}

int	builtin_exit(t_command *cmd, t_shell *shell)
{
	long long	value;

	if (!cmd || !shell)
		return (1);
	if (!cmd->argv[1])
		exit_with_no_arg(shell);
	if (!ft_strtoll(cmd->argv[1], &value))
		argument_validation(cmd, shell);
	if (cmd->argv[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->last_status = 1;
		return (1);
	}
	shell->last_status = (unsigned char)value;
	if (shell->in_child)
	{
		cleanup(shell);
		free(shell->pipeline.child_pids);
		_exit(shell->last_status);
	}
	if (is_interactive(shell))
		ft_putstr_fd("exit\n", 1);
	cleanup(shell);
	exit(shell->last_status);
}

int	exec_exit_in_parent(t_command *cmd, t_shell *shell)
{
	int	saved[2];

	if (!cmd || !shell)
		return (1);
	shell->in_child = 0;
	if (apply_redirs_in_parent(cmd, saved) != 0)
	{
		return (1);
	}
	shell->last_status = builtin_exit(cmd, shell);
	restore_stdio(saved);
	return (shell->last_status);
}
