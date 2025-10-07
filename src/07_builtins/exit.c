/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:55:49 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/07 07:47:48 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	is_interactive(t_shell *shell)
{
	if (shell && shell->interactive)
		return (1);
	return (isatty(STDIN_FILENO));
}

static void	cleanup(t_shell *shell)
{
	free_allocs(shell);
	free_split(&shell->env_arr);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
}

int	builtin_exit(t_command *cmd, t_shell *shell)
{
	int			status;
	long long	value;

	if (!cmd || !shell)
		return (1);
	if (!cmd->argv[1]) //only EXIT
	{
		status = (unsigned char)shell->last_status;
		if (shell->in_child)
		{
			cleanup(shell);
			free(shell->pipeline.child_pids);
			_exit(status);
		}
		if (is_interactive(shell))
			ft_putstr_fd("exit\n",1);
		cleanup(shell);
		exit(status);
	}
	if (!ft_strtoll(cmd->argv[1], &value)) //ARGUMENT VALIDATION
	{
		if (!shell->in_child && is_interactive(shell))
			ft_putstr_fd("exit\n", 1);
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(cmd->argv[1] ,2);
		ft_putstr_fd(": numeric argument required\n", 2);
		cleanup(shell);
		if (shell->in_child)
		{
			free(shell->pipeline.child_pids);
			_exit(2);
		}
		exit(2);
	}
	if (cmd->argv[2]) // TOO MANY ARGS
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		shell->last_status = 1;
		return(1);
	}
	status = (unsigned char)value;
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

int exec_exit_in_parent(t_command *cmd, t_shell *shell)
{
    int saved[2];

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

