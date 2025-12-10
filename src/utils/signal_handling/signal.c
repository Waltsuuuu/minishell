/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 09:18:40 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/15 13:21:17 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Handle incoming signals during a prompt.
 * Resets the input line, and display fresh prompt.
 * 
 * @param signum, received signal number(e.g SIGINT)
 */
void	handle_sig(int signum)
{
	g_signal = signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
}

/**
 * @brief Setup signal handlers for minishell prompt
 * Configures SIGINT to call handle_sig and ignores SIGQUIT
 */
void	setup_signal_handlers_for_prompt(void)
{
	struct sigaction	sa_prompt;

	ft_bzero(&sa_prompt, sizeof(sa_prompt));
	rl_catch_signals = 0;
	rl_catch_sigwinch = 0;
	sigemptyset(&sa_prompt.sa_mask);
	sa_prompt.sa_flags = SA_RESTART;
	sa_prompt.sa_handler = handle_sig;
	sigaction(SIGINT, &sa_prompt, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Setup default signal handlers for child processes.
 *
 * Restores default handling of SIGINT and SIGQUIT for child
 * processes so they behave like in a normal shell execution.
 */
void	setup_signal_handlers_for_child(void)
{
	struct sigaction	sa_child;

	ft_bzero(&sa_child, sizeof(sa_child));
	sa_child.sa_handler = SIG_DFL;
	sigemptyset(&sa_child.sa_mask);
	sigaction(SIGINT, &sa_child, NULL);
	sigaction(SIGQUIT, &sa_child, NULL);
}

// Ignore SIGINT and SIGQUIT in parent during waitpid().
void	ignore_parent_sig_handlers(struct sigaction *ign,
	struct sigaction *old_quit,
	struct sigaction *old_int)
{
	ft_bzero(ign, sizeof(*ign));
	ign->sa_handler = SIG_IGN;
	sigemptyset(&ign->sa_mask);
	sigaction(SIGINT, ign, old_int);
	sigaction(SIGQUIT, ign, old_quit);
}

// Restore SIGINT and SIGQUIT handlers in parent 
//(stop ignoring) after waitpid().
void	restore_parent_sig_handlers(struct sigaction *old_quit,
	struct sigaction *old_int)
{
	sigaction(SIGINT, old_int, NULL);
	sigaction(SIGQUIT, old_quit, NULL);
}
