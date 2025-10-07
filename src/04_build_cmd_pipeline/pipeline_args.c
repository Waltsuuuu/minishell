/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_args.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 18:43:19 by wheino            #+#    #+#             */
/*   Updated: 2025/10/07 18:46:39 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Appends a TOK_WORD token.text to a temporary
 * linked list of argments for a command segment.
 * @return 0 on success, -1 on alloc error.
 */
int	append_arg(t_token *tokens, int i, t_seg *seg)
{
	t_list	*node;
	char	*arg;

	arg = ft_strdup(tokens[i].text);
	if (!arg)
		return (-1);
	node = ft_lstnew(arg);
	if (!node)
	{
		free(arg);
		return (-1);
	}
	ft_lstadd_back(&seg->args, node);
	return (0);
}

/**
 * Move 'seg->args' linked list nodes content
 * into 'pipeline->cmds[cmd_i].argv' array.
 * Frees the node, but not the content.
 * Null-terminates argv and clears seg->args.
 * @return 0 on success, -1 on alloc error.
 */
int	arg_ll_to_arr(t_seg *seg, t_pipeline *pipeline, int cmd_i)
{
	t_list	*current;
	t_list	*next;
	int		i;

	pipeline->cmds[cmd_i].argv = malloc(sizeof(char *) * (seg->argc + 1));
	if (!pipeline->cmds[cmd_i].argv)
		return (-1);
	current = seg->args;
	i = 0;
	while (current)
	{
		pipeline->cmds[cmd_i].argv[i] = current->content;
		next = current->next;
		free(current);
		current = next;
		i++;
	}
	seg->args = NULL;
	pipeline->cmds[cmd_i].argv[i] = NULL;
	return (0);
}
