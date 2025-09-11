#include "minishell.h"

int	build_and_append_redir(t_token *tokens, int i, t_seg *seg)
{
	t_redir			*redir;
	t_list			*node;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (-1);
	if (get_redir_type(tokens[i].type, &redir->type) == -1)	// set type
	{
		free(redir);
		return (-1);
	}
	redir->target = ft_strdup(tokens[i + 1].text);			// set target
	if (!redir->target)
	{
		free(redir);
		return (-1);
	}
	if (redir->target[0] == '\0' && redir->type != REDIR_HEREDOC)	// empty target check (Can be empty for heredoc)
	{
		free(redir->target);
		free(redir);
		printf("minishell: ambiguous redirect\n");
		return (-1);
	}
	redir->no_expand = 0;												// set no_expand
	if (redir->type == REDIR_HEREDOC && tokens[i + 1].was_quoted == 1)	// set no_expand 1 if heredoc and delim was quoted
		redir->no_expand = 1;
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