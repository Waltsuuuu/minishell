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
int	exec_dispatch(char **envp, t_pipeline *pipeline, t_shell *shell)
{
	if (is_single_export(pipeline))
		return (exec_export_in_parent(&pipeline->cmds[0], shell));
	return (exec_pipeline(envp, pipeline, shell));
}

/**
 * Internal helper: returns 1 if pipeline is a single "export".
 *
 * @param p pipeline
 * @return 1 if exactly one command and it is "export", else 0
 */
int	is_single_export(const t_pipeline *p)
{
	if (!p || p->n_cmds != 1)
		return (0);
	return (p->cmds && p->cmds[0].argv && p->cmds[0].argv[0]
		&& ft_strcmp(p->cmds[0].argv[0], "export") == 0);
}