#include "minishell.h"

void	free_allocs(t_shell *shell)
{
	clear_struct_on_failure(&shell->input);
	free_pipeline(&shell->pipeline, shell->pipeline.n_cmds);
	if (shell->pipeline.cmds)
		free(shell->pipeline.cmds);
	shell->pipeline.cmds = NULL;
	free(shell->buf);
}