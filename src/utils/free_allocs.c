#include "minishell.h"

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