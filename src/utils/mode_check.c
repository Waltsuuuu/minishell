/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_check.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 18:21:39 by wheino            #+#    #+#             */
/*   Updated: 2025/10/09 19:25:26 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// interactive: readline(prompt) - Reading from terminal
// non-interactive: get_next_line(STDIN_FILENO) + remove '\n'
// Returns: malloc'd line or NULL on EOF/error.
char	*read_with_mode_check(const char *buf)
{
	char	*line;

	if (isatty(STDIN_FILENO))
		return (readline(buf));
	line = get_next_line(STDIN_FILENO);
	if (!line)
		return (NULL);
	remove_nl(line);
	return (line);
}

void	remove_nl(char *str)
{
	int	len;

	if (!str)
		return ;
	len = ft_strlen(str);
	if (len > 0 && str[len - 1] == '\n')
		str[len - 1] = '\0';
}
