#include "minishell.h"

int	err_exit_build_pipeline(t_pipeline *pipeline, t_seg *seg, int built_count)
{
	free_partial_seg(seg);
	free_partial_pipeline();
	if (pipeline && pipeline->cmds)
	{
		free(pipeline->cmds);
		pipeline->cmds = NULL;
	}
	if (pipeline)
		pipeline->n_cmds = 0;
	return (-1);
}

void	free_partial_seg(t_seg *seg)
{
	if (!seg)
		return ;
	if (seg->args)
		ft_lstclear(&seg->args, free);
	if (seg->redirs)
		ft_lstclear(&seg->redirs, del_redir);
	seg->argc = 0;
}

void	del_redir(void *content)
{
	t_redir *redir;

	if (!content)
		return ;
	redir = (t_redir *)content;
	if (redir->target)
		free(redir->target);
	free(redir);
}

