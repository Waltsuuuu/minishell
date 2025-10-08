/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tty.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 09:19:04 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/08 09:19:05 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// If stdin is a TTY, read and store its current termios settings
// so they can be restored later.
// Reason: Interupted readline() can leave tty in non-echo mode.
void	save_terminal_state(struct termios *tty)
{
	if (isatty(STDIN_FILENO))
		tcgetattr(STDIN_FILENO, tty);
}

// If stdin is a TTY, restore the termios settings from the TTY struct.
// TCSANOW = Apply settings immeditaely 
void	restore_terminal_state(struct termios *tty)
{
	if (isatty(STDIN_FILENO))
		tcsetattr(STDIN_FILENO, TCSANOW, tty);
}
