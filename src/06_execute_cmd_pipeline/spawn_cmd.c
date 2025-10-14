/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   spawn_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 07:03:12 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/14 13:57:19 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	child_cleanup_and_exit(t_shell *shell, int status)
{
	free_allocs(shell);
	free(shell->pipeline.child_pids);
	clean_env(&shell->env_head);
	free_split(&shell->env_arr);
	_exit(status);
}

static void	maybe_run_builtin_and_exit(t_command *cmd, t_shell *shell)
{
	if (cmd && cmd->argv && cmd->argv[0] && is_builtin_name(cmd->argv[0]))
	{
		shell->last_status = run_builtin(cmd, shell);
		child_cleanup_and_exit(shell, shell->last_status);
	}
}

static void	process_all_redirs(t_list *redirs, int *final_in,
								int *final_out, t_shell *shell)
{
	t_list	*node;
	t_redir	*redir;

	node = redirs;
	while (node)
	{
		redir = (t_redir *)node->content;
		handle_redir_out(redir, final_out, shell);
		handle_redir_append(redir, final_out, shell);
		handle_redir_in(redir, final_in, shell);
		handle_redir_heredoc(redir, final_in, shell);
		node = node->next;
	}
}

pid_t	spawn_cmd(t_command *cmd, int pipe_in, int pipe_out, t_shell *shell)
{
	pid_t	pid;
	int		final_in;
	int		final_out;

	pid = fork();
	if (pid == 0)
	{
		// CHILD Fix! Close all redir->hd_fds that do not belong to this command.
		shell->in_child = 1;
		setup_signal_handlers_for_child();
		set_child_fds_from_pipes(&final_in, &final_out, pipe_in, pipe_out);
		process_all_redirs(cmd->redirs, &final_in, &final_out, shell);
		replug_child_stdin(final_in);
		replug_child_stdout(final_out);
		child_finalize_pipes(shell);
		maybe_run_builtin_and_exit(cmd, shell);
		exec_with_path_search(cmd->argc, cmd->argv, shell);
		_exit(127);
	}
	return (pid);
}
