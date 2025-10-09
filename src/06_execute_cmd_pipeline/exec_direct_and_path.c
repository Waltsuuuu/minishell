/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_direct_and_path.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 09:29:38 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/09 09:30:20 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	path_exec(char **argv, t_shell *shell)
{
	char	path[PATH_MAX];
	char	*temp;
	char	*full;
	int		err;

	if (!argv || !argv[0])
		_exit(0);
	if (!getcwd(path, sizeof(path)))
		_exit(1);
	temp = ft_strjoin(path, "/");
	if (!temp)
		_exit(1);
	full = ft_strjoin(temp, argv[0]);
	free (temp);
	if (!full)
		_exit(1);
	execve(full, argv, shell->env_arr);
	err = errno;
	free (full);
	execve_error_and_exit(shell, argv, shell->pipeline.child_pids, err);
}

void	exec_with_candidate_path(char **argv, char **path_dirs, t_shell *s)
{
	char	*candidate_path;
	int		path_index;

	path_index = 0;
	while (path_dirs[path_index])
	{
		candidate_path = join_cmd_to_path(path_dirs[path_index],
				argv[0]);
		if (!candidate_path)
		{
			clean(path_dirs, s, s->pipeline.child_pids);
			_exit(1);
		}
		execve(candidate_path, argv, s->env_arr);
		free(candidate_path);
		path_index++;
	}
}

void	exec_with_path_search(int argc, char **argv, t_shell *shell)
{
	char	**path_directories;

	path_directories = NULL;
	if (argv && argv[0])
	{
		if (has_slash(argv[0]))
			direct_exec(argv, shell, shell->pipeline.child_pids);
		path_directories = find_from_path(shell->env_arr);
		if (!path_directories)
			path_exec(argv, shell);
	}
	if (argv && argv[0] && path_directories)
	{
		exec_with_candidate_path(argv, path_directories, shell);
	}
	if (argc != 0)
	{
		write(2, argv[0], (int)strlen(argv[0]));
		write(2, ": command not found\n", 20);
	}
	clean(path_directories, shell, shell->pipeline.child_pids);
	_exit(127);
}