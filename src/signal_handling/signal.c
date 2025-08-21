/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/21 13:48:09 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/21 14:40:28 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

volatile sig_atomic_t g_signal = 0;

void	handle_sig(int signum)
{
	g_signal = signum;
}

void	setup_signal_handlers_for_prompt()
{
	struct	sigaction sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = handle_sig;
	
	sigaction(SIGINT, &sa, NULL); //Ctrl + C
	sigaction(SIGQUIT, &sa, NULL); //Ctrl + \'
}