/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_redirs.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:48:58 by wheino            #+#    #+#             */
/*   Updated: 2025/10/07 18:57:58 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Fills a t_redir, stores it in a node and
 * appends it to seg->redirs linked list.
 * @return 0 on success, -1 on syntax/alloc error.
 */
int	build_and_append_redir(t_token *tokens, int i, t_seg *seg)
{
	t_redir			*redir;
	t_list			*node;

	redir = ft_calloc(1, sizeof(t_redir));
	if (!redir)
		return (-1);
	redir->hd_fd = -1;
	if (fill_redir_fields(redir, tokens, i) == -1)
	{
		if (redir->target)
			free(redir->target);
		free(redir);
		return (-1);
	}
	node = ft_lstnew(redir);
	if (!node)
	{
		free(redir->target);
		free(redir);
		return (-1);
	}
	ft_lstadd_back(&seg->redirs, node);
	return (0);
}

/**
 * Set t_redir type/target/no_expand fields from
 * tokens[i] (redirect) and tokens[i+1] (target).
 * Allows empty target for HEREDOC.
 * @return 0 on success, -1 on error.
 */
int	fill_redir_fields(t_redir *redir, t_token *tokens, int i)
{
	if (get_redir_type(tokens[i].type, &redir->type) == -1)
		return (-1);
	redir->target = ft_strdup(tokens[i + 1].text);
	if (!redir->target)
		return (-1);
	if (redir->target[0] == '\0' && redir->type != REDIR_HEREDOC)
	{
		ft_putstr_fd("minishell: ambiguous redirect\n", 2);
		return (-1);
	}
	if (redir->type == REDIR_HEREDOC && tokens[i + 1].was_quoted == 1)
		redir->no_expand = 1;
	return (0);
}
