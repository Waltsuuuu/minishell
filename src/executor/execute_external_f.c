/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_external_f.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 12:35:45 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/21 12:37:52 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	exec_ext_func(char **absolute_paths, char **words, char **envp)
{
	size_t	counter;
	pid_t	pid;
	int 	status;
	
	counter = 0;
		while (absolute_paths && absolute_paths[counter])
		{
			if (access(absolute_paths[counter], X_OK) == 0)
			{
				pid = fork();
				if (pid == 0)
				{
					execve(absolute_paths[counter], words, envp);
					perror("execve");
					_exit(127);
				}
				waitpid(pid, &status, 0);
				break;
			}
			counter++;
		}
		if (!absolute_paths || !absolute_paths[counter])
			write(2, "minishell: command not found\n", 29);
}