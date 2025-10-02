#include "minishell.h"

/**
 * Decides whether to run a single builtin in the parent, otherwise
 * falls back to normal pipeline execution.
 *
 * @param envp  environment vector for external execs
 * @param pipeline parsed pipeline
 * @param shell shell state (last_status, env, etc.)
 * @return exit status of the executed command(s)
 */
int	exec_dispatch(t_pipeline *pipeline, t_shell *shell)
{
	if (pipeline->n_cmds == 1
		&& pipeline->cmds[0].argv
		&& pipeline->cmds[0].argv[0]
		&& is_parent_builtin(pipeline->cmds[0].argv[0]))
	{
		if (ft_strcmp(pipeline->cmds[0].argv[0], "export") == 0)
			return (exec_export_in_parent(&pipeline->cmds[0], shell));
		else if (ft_strcmp(pipeline->cmds[0].argv[0], "unset") == 0)
			return (exec_unset_in_parent(&pipeline->cmds[0], shell));
		else if (ft_strcmp(pipeline->cmds[0].argv[0], "cd") == 0)
			return (exec_cd_in_parent(&pipeline->cmds[0], shell));
		else if (ft_strcmp(pipeline->cmds[0].argv[0], "exit") == 0)
			return (exec_exit_in_parent(&pipeline->cmds[0], shell));
	}
	return (exec_pipeline(pipeline, shell));
}
