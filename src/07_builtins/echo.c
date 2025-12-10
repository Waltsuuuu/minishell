/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:06:35 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/06 12:22:36 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_argv(t_command *cmd, int argv_index)
{
	while (cmd->argv[argv_index])
	{
		ft_putstr_fd(cmd->argv[argv_index], 1);
		if (cmd->argv[argv_index + 1])
			ft_putstr_fd(" ", 1);
		argv_index++;
	}
}

int	builtin_echo(t_command *cmd)
{
	int	i;
	int	j;
	int	nflag;

	i = 1;
	nflag = 0;
	if (!cmd || !cmd->argv || !cmd->argv[0])
		return (0);
	while (cmd->argv[i] && cmd->argv[i][0] == '-' && cmd->argv[i][1] == 'n')
	{
		j = 1;
		while (cmd->argv[i][j] == 'n')
			j++;
		if (cmd->argv[i][j] != '\0')
			break ;
		nflag = 1;
		i++;
	}
	print_argv(cmd, i);
	if (!nflag)
		ft_putstr_fd("\n", 1);
	return (0);
}

int	exec_echo_in_parent(t_command *cmd, t_shell *shell)
{
	int	saved[2];

	if (!cmd || !shell)
		return (1);
	if (apply_redirs_in_parent(cmd, saved) != 0)
		return (1);
	shell->last_status = builtin_echo(cmd);
	restore_stdio(saved);
	return (shell->last_status);
}
