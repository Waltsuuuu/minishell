/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:48:09 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/22 14:53:09 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

void	handle_sig(int signum)
{
	g_signal = signum;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	
}

void	setup_signal_handlers_for_prompt()
{
	struct	sigaction sa_prompt;
	
	ft_bzero(&sa_prompt, sizeof(sa_prompt));
	rl_catch_signals = 0; //these 2 lines overwrites the readlines own handlers
	rl_catch_sigwinch = 0;

	sigemptyset(&sa_prompt.sa_mask);
	sa_prompt.sa_flags = SA_RESTART;
	sa_prompt.sa_handler = handle_sig;
	
	sigaction(SIGINT, &sa_prompt, NULL); //Ctrl + C
	signal(SIGQUIT, SIG_IGN); //Ctrl + \'
}