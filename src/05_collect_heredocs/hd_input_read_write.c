/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_input_read_write.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:17:50 by wheino            #+#    #+#             */
/*   Updated: 2025/10/09 12:37:08 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	readline_and_check_eof(t_hd_state *state, t_redir *redir)
{
	state->line = readline("heredoc>");
	if (!state->line)
		return (1);
	if (ft_strcmp(state->line, redir->target) == 0)
	{
		free(state->line);
		return (1);
	}
	return (0);
}

int	handle_heredoc_line(t_hd_state *state, char *line,
		t_redir *redir, t_shell *shell)
{
	if (redir->no_expand == 0)  //Delim was n
	{
		if (expand_write_line(state, line, shell) == -1)
			return (-1);
	}
	else
		write_line_nl(state->fds[1], line);
	return (0);
}

int	expand_write_line(t_hd_state *state, char *line, t_shell *shell)
{
	char	*temp1;
	char	*temp2;

	temp1 = hd_expand_status(line, shell->last_status);
	if (!temp1)
		return (-1);
	temp2 = expand_variable(temp1, shell->env_arr);
	if (!temp2)
	{
		free(temp1);
		return (-1);
	}
	state->pipe_bytes_written += ft_strlen(temp2);
	if (state->pipe_bytes_written > PIPE_BUF_MAX)
	{
		free(temp1);
		free(temp2);
		ft_putstr_fd("minishell: Error! Heredoc max size 63KiB\n", 2);
		return (-1);
	}
	write_line_nl(state->fds[1], temp2);
	free(temp1);
	free(temp2);
	return (0);
}

void	write_line_nl(int fd, char *line)
{
	if (!line)
		return ;
	write(fd, line, ft_strlen(line));
	write(fd, "\n", 1);
}
