/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exit_free.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:47:18 by wheino            #+#    #+#             */
/*   Updated: 2025/10/14 14:19:22 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	err_exit_build_pipeline(t_pipeline *pipeline, t_seg *seg, int built_count)
{
	free_partial_seg(seg);
	if (pipeline && pipeline->cmds)
	{
		free_pipeline(pipeline, built_count);
		free(pipeline->cmds);
		pipeline->cmds = NULL;
	}
	if (pipeline)
		pipeline->n_cmds = 0;
	return (-1);
}

void	free_pipeline(t_pipeline *pipeline, int cmd_count)
{
	int	i;
	int	j;

	i = 0;
	while (i < cmd_count)
	{
		if (pipeline->cmds[i].argv)
		{
			j = 0;
			while (pipeline->cmds[i].argv[j])
			{
				free(pipeline->cmds[i].argv[j]);
				j++;
			}
			free(pipeline->cmds[i].argv);
			pipeline->cmds[i].argv = NULL;
		}
		if (pipeline->cmds[i].redirs)
		{
			ft_lstclear(&pipeline->cmds[i].redirs, del_redir);
			pipeline->cmds[i].redirs = NULL;
		}
		pipeline->cmds[i].argc = 0;
		i++;
	}
}

void	free_partial_seg(t_seg *seg)
{
	if (!seg)
		return ;
	if (seg->args)
	{
		ft_lstclear(&seg->args, free);
		seg->args = NULL;
	}
	if (seg->redirs)
	{
		ft_lstclear(&seg->redirs, del_redir);
		seg->redirs = NULL;
	}
	seg->argc = 0;
}

void	del_redir(void *content)
{
	t_redir	*redir;

	if (!content)
		return ;
	redir = (t_redir *)content;
	if (redir->hd_fd != -1)
		close(redir->hd_fd);
	if (redir->target)
		free(redir->target);
	free(redir);
}
