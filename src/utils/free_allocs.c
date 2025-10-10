/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_allocs.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 18:21:28 by wheino            #+#    #+#             */
/*   Updated: 2025/10/10 12:35:34 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_shell(t_shell *shell)
{
	free_str_ptr(&shell->buf);
	free_str_ptr(&shell->line);
	free_allocs(shell);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
	clear_history();
}

void	cleanup(t_shell *shell)
{
	free_allocs(shell);
	free_split(&shell->env_arr);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
}

void	free_on_failure(t_shell *shell)
{
	free_str_ptr(&shell->line);
	free_allocs(shell);
	free_split(&shell->env_arr);
}

void	free_allocs(t_shell *shell)
{
	clear_struct_on_failure(&shell->input);
	if (shell->pipeline.cmds)
	{
		free_pipeline(&shell->pipeline, shell->pipeline.n_cmds);
		free(shell->pipeline.cmds);
	}
	shell->pipeline.cmds = NULL;
}

void	free_str_ptr(char **str_ptr)
{
	if (str_ptr != NULL && *str_ptr != NULL)
	{
		free(*str_ptr);
		*str_ptr = NULL;
	}
}
