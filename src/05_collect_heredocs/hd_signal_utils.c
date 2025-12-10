/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_signal_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:18:00 by wheino            #+#    #+#             */
/*   Updated: 2025/10/08 18:10:43 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_child_sighandler(void)
{
	struct sigaction	sa;

	ft_bzero(&sa, sizeof(sa));
	sa.sa_handler = close_stdin_on_sigint;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
}

void	close_stdin_on_sigint(int sig_num)
{
	g_signal = sig_num;
	close(STDIN_FILENO);
}

int	wait_child(t_hd_state *state)
{
	while (1)
	{
		state->wait_result = waitpid(state->pid, &state->status, 0);
		if (state->wait_result == -1 && errno == EINTR)
			continue ;
		break ;
	}
	if (state->wait_result == -1)
		return (-1);
	return (0);
}

int	handle_child_status(t_hd_state *state, t_shell *shell)
{
	if (WIFSIGNALED(state->status) && WTERMSIG(state->status) == SIGINT)
	{
		close(state->fds[0]);
		shell->last_status = 130;
		return (-1);
	}
	if (!WIFEXITED(state->status) || WEXITSTATUS(state->status) != 0)
	{
		close(state->fds[0]);
		return (-1);
	}
	return (0);
}
