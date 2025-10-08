/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hd_input_read_write.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 17:17:50 by wheino            #+#    #+#             */
/*   Updated: 2025/10/08 17:17:51 by wheino           ###   ########.fr       */
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

int	handle_heredoc_line(t_hd_state *state, int fd, char *line, t_redir *redir, int last_status, char **envp)
{
	if (redir->no_expand == 0)
	{
		if (expand_write_line(state, fd, line, last_status, envp) == -1)
			return (-1);
	}
	else
		write_line_nl(fd, line);
	return (0);
}

int	expand_write_line(t_hd_state *state, int fd, char *line, int last_status, char **envp)
{
	char *temp1;
	char *temp2;

	temp1 = expand_status(line, last_status);
	if (!temp1)
		return (-1);
	temp2 = expand_variable(temp1, envp);
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
	write_line_nl(fd, temp2);
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
