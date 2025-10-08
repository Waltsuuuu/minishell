/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_parse.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 18:17:58 by wheino            #+#    #+#             */
/*   Updated: 2025/10/08 18:32:26 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_tokenize(t_shell *shell)
{
	if (check_quote_balance(&shell->line, &shell->last_status) == -1)
	{
		free_str_ptr(&shell->line);
		return (-1);
	}
	add_history(shell->line);
	if (parse_input_line(shell->line, &shell->input) == -1)
	{
		free_on_failure(shell);
		return (-1);
	}
	if (count_heredocs(&shell->input) >= 16)
	{
		ft_putstr_fd("minishell: maximum here-document count exceeded\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

int	parse_rm_quotes_and_expand(t_shell *shell)
{
	if (expand_tokens(&shell->input, shell->last_status, shell->env_arr) == -1)
	{
		free_on_failure(shell);
		return (-1);
	}
	if (remove_quotes(&shell->input) == -1)
	{
		free_on_failure(shell);
		return (-1);
	}
	if (shell->input.n_tokens <= 0)
	{
		free_on_failure(shell);
		return (-1);
	}
	return (0);
}

int	build_pipeline_collect_hd(t_shell *shell)
{
	if (build_pipeline(shell, &shell->input,
			shell->input.tokens, &shell->pipeline) == -1)
	{
		free_on_failure(shell);
		return (-1);
	}
	if (collect_heredocs(&shell->pipeline, shell) == -1)
	{
		free_on_failure(shell);
		return (-1);
	}
	return (0);
}
