/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_direct_and_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:29:38 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/17 11:28:55 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	clean_and_exit_num(t_shell *shell, int exit_num)
{
	if (shell->pipeline.child_pids)
		free(shell->pipeline.child_pids);
	cleanup(shell);
	_exit(exit_num);
}

void	path_exec(char **argv, t_shell *shell)
{
	char	path[PATH_MAX];
	char	*temp;
	char	*full;
	int		err;

	if (!argv || !argv[0])
		clean_and_exit_num(shell, 0);
	if (!getcwd(path, sizeof(path)))
		clean_and_exit_num(shell, 1);
	temp = ft_strjoin(path, "/");
	if (!temp)
		clean_and_exit_num(shell, 1);
	full = ft_strjoin(temp, argv[0]);
	free (temp);
	if (!full)
		clean_and_exit_num(shell, 1);
	execve(full, argv, shell->env_arr);
	err = errno;
	free (full);
	execve_error_and_exit(shell, argv, shell->pipeline.child_pids, err);
}

void	exec_with_candidate_path(char **argv, char **dirs, t_shell *s, int *err)
{
	char	*candidate_path;
	int		path_index;

	path_index = 0;
	while (dirs[path_index])
	{
		candidate_path = join_cmd_to_path(dirs[path_index],
				argv[0]);
		if (!candidate_path)
		{
			clean(dirs, s, s->pipeline.child_pids);
			_exit(1);
		}
		execve(candidate_path, argv, s->env_arr);
		*err = errno;
		if ((*err != ENOENT && *err != ENOTDIR)
			|| access(candidate_path, F_OK) == 0)
		{
			free(candidate_path);
			break ;
		}
		free(candidate_path);
		path_index++;
	}
}

static void	check_dot_dotdot(char **argv, t_shell *shell, char **path_dirs)
{
	if (!argv || !argv[0])
		return ;
	if (!ft_strcmp(argv[0], ".") || !ft_strcmp(argv[0], ".."))
	{
		ft_putstr_fd(argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		clean(path_dirs, shell, shell->pipeline.child_pids);
		_exit(127);
	}
	else if (!(ft_strcmp(argv[0], "")))
	{
		ft_putstr_fd(argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		clean(path_dirs, shell, shell->pipeline.child_pids);
		_exit(127);
	}
}

void	exec_with_path_search(int argc, char **argv, t_shell *shell)
{
	char	**path_directories;
	int		err;

	err = 0;
	path_directories = NULL;
	if (argv && argv[0])
	{
		check_dot_dotdot(argv, shell, path_directories);
		if (has_slash(argv[0]))
			direct_exec(argv, shell, shell->pipeline.child_pids);
		path_directories = find_from_path(shell->env_arr);
		if (!path_directories)
			path_exec(argv, shell);
	}
	if (argv && argv[0] && path_directories)
		exec_with_candidate_path(argv, path_directories, shell, &err);
	if (argc == 0)
	{
		clean(path_directories, shell, shell->pipeline.child_pids);
		_exit (0);
	}
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(argv[0], STDERR_FILENO);
	env_path_execve_error_and_exit(shell, path_directories, err);
	_exit(127);
}
