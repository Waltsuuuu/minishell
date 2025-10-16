/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 12:04:50 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/16 13:03:23 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cd_error_and_return(char **argv)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(argv[0], STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	if (errno == ENOENT)
	{
		ft_putstr_fd("No such file or directory\n", STDERR_FILENO);
		return (1);
	}
	else if (errno == EACCES)
	{
		ft_putstr_fd("No permission\n", STDERR_FILENO);
		return (1);
	}
	else if (errno == ENOTDIR)
	{
		ft_putstr_fd(": is not a directory\n", STDERR_FILENO);
		return (1);
	}
	else
	{
		ft_putstr_fd("Undefined cd error\n", STDERR_FILENO);
		return (1);
	}
}

int	builtin_cd(char **argv, t_shell *shell)
{
	char	oldbuf[PATH_MAX];
	char	newbuf[PATH_MAX];
	char	*path;

	if (argv[1] && argv[2])
		return (ft_putstr_fd("cd expects exactly one path.\n", 2), 1);
	if (argv[0] && !argv[1])
	{
		path = env_get(shell, "HOME");
		if (!path)
			return (ft_putstr_fd("minishell: cd: HOME not set\n", 2), 1);
	}
	else
		path = argv[1];
	if (!getcwd(oldbuf, sizeof(oldbuf)))
		ft_putstr_fd("cd: getwcd\n", 2);
	if (chdir(path) < 0)
		return (cd_error_and_return(argv));
	if (!getcwd(newbuf, sizeof(newbuf)))
		return (ft_putstr_fd("getwcd error\n", 2), 1);
	if (env_set(&shell->env_head, "OLDPWD", oldbuf) == 1)
		return (ft_putstr_fd("oldpwd update error\n", 2), 1);
	if (env_set(&shell->env_head, "PWD", newbuf) == 1)
		return (ft_putstr_fd("pwd update error\n", 2), 1);
	return (0);
}

int	exec_cd_in_parent(t_command *cmd, t_shell *shell)
{
	int	saved[2];

	if (!cmd || !shell)
		return (1);
	if (apply_redirs_in_parent(cmd, saved) != 0)
	{
		return (1);
	}
	shell->last_status = builtin_cd(cmd->argv, shell);
	restore_stdio(saved);
	return (shell->last_status);
}
