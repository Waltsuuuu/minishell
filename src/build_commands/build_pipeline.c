#include "minishell.h"
// Remember to init t_pipeline *pipeline to 0 / NULL before passing it to this func

typedef struct s_seg {
	t_list *args;     /* list of char* */
	int     argc;     /* count of args */
	t_list *redirs;   /* list of t_redir* */
	int     start;    /* token index at segment start (for “empty” check) */
} t_seg;

int	build_pipeline(t_input *input, t_token *tokens, t_pipeline *pipeline)
{
	int		n_cmds;
	int		i;
	int		cmd_i;
	t_seg	seg;

	n_cmds = count_pipes(tokens) + 1;
	pipeline->cmds = malloc(sizeof(t_command) * n_cmds);
	if (!pipeline->cmds)
		return (-1);
	i = 0;
	if (tokens[i].type == TOK_PIPE)
	{
		ft_printf("Syntax error near '%s' at position %d\n", tokens[i].text, tokens[i].pos); // Helper for errors later
		return (-1);
	}
	cmd_i = 0;
	i = 0;
	while (cmd_i < n_cmds)
	{
		seg.argc = 0;
		seg.start = 0;
		seg.args = NULL;
		seg.redirs = NULL;
		while ((i < input->n_tokens) && tokens[i].type != TOK_PIPE)
		{
			if (is_redir_tok(tokens[i].type))
			{
				if ((i + 1 >= input->n_tokens) || tokens[i + 1].type != TOK_WORD)
				{
					ft_printf("Syntax error near '%s' at position %d\n", tokens[i].text, tokens[i].pos);
					// Clean up function for already allocated memory here?? Or idk maybe not.
					return (-1);
				}
				if (build_and_append_redir(input, tokens, i, &seg) == -1)
				{
					// Possible clean up here too?
					return (-1);
				}
				i += 2; // (2 becuase hanled redir + target)
				continue ; 
			}
			else // TokType == TOK_WORD
			{
				// make node and set content to token.text
				// append node to seg.args
				// seg.argc++;
				// i++;
				// continue ;
			}		
		}
	}
}

int	build_and_append_redir(t_input *input, t_token *tokens, int i, t_seg *seg)
{
	t_redir			*redir;
	t_list			*node;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (-1);
	if (get_redir_type(tokens[i].type, &redir->type) == -1)	// set type
		return (-1);
	redir->target = ft_strdup(tokens[i + 1].text);			// set target
	if (!redir->target)
		return (-1);
	if (redir->target[0] == '\0' && redir->type != REDIR_HEREDOC)	// empty target check (Can be empty for heredoc)
	{
		free(redir->target);
		ft_printf("minishell: ambiguous redirect\n");
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

static int	get_redir_type(TokType type, t_redir_type *t_type)
{
	if (type == TOK_REDIR_IN)
		*t_type = REDIR_IN;
	else if (type == TOK_REDIR_OUT)
		*t_type = REDIR_OUT;
	else if (type == TOK_REDIR_APP)
		*t_type = REDIR_APPEND;
	else if (type == TOK_HEREDOC)
		*t_type = REDIR_HEREDOC;
	else
		return (-1);
	return (0);
}

static int	is_redir_tok(TokType type)
{
	if (type == TOK_HEREDOC || type == TOK_REDIR_IN
			|| type == TOK_REDIR_OUT || type == TOK_REDIR_APP)
		return (1);
	return (0);
}

static int	count_pipes(t_token *tokens)
{
	int count;
	int	i;

	if (!tokens)
		return (0);
	count = 0;
	i = 0;
	while (tokens)
	{
		if (tokens[i].type == TOK_PIPE)
			count++;
		i++;
	}
	return (count);
}
