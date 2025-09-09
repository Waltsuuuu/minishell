#include "minishell.h"
// Remember to init t_pipeline *pipeline to 0 / NULL before passing it to this func

int	build_pipeline(t_input *input, t_token *tokens, t_pipeline *pipeline)
{
	int		n_cmds;
	int		i;
	int		cmd_i;
	t_seg	seg;

	n_cmds = count_pipes(input) + 1;
	pipeline->cmds = malloc(sizeof(t_command) * n_cmds);
	if (!pipeline->cmds)
		return (-1);
	i = 0;
	if (tokens[i].type == TOK_PIPE)
	{
		printf("Syntax error near '%s' at position %d\n", tokens[i].text, tokens[i].pos); // Helper for errors later
		return (-1);
	}
	cmd_i = 0;
	i = 0;
	while (cmd_i < n_cmds)
	{
		seg.argc = 0;
		seg.start = i;
		seg.args = NULL;
		seg.redirs = NULL;
		while ((i < input->n_tokens) && tokens[i].type != TOK_PIPE)
		{
			if (is_redir_tok(tokens[i].type))
			{
				if ((i + 1 >= input->n_tokens) || tokens[i + 1].type != TOK_WORD)
				{
					printf("Syntax error near '%s' at position %d\n", tokens[i].text, tokens[i].pos);
					// Clean up function for already allocated memory here?? Or idk maybe not.
					return (-1);
				}
				if (build_and_append_redir(tokens, i, &seg) == -1)
				{
					// Possible clean up here too?
					return (-1);
				}
				i += 2; // (2 becuase hanled redir + target)
				continue ; 
			}
			else if (tokens[i].type == TOK_WORD)
			{
				if (append_arg(tokens, i, &seg) == -1)
				{
					// clean up 
					return (-1);
				}
				seg.argc++;
				i++;
				continue ;
			}
			else
			{
				printf("Syntax error near '%s' at position %d\n", tokens[i].text, tokens[i].pos);
				return (-1);
			}
		}
		if (arg_ll_to_arr(&seg, pipeline, cmd_i) == -1)
		{
			// clean up
			return (-1);
		}
		pipeline->cmds[cmd_i].argc = seg.argc;
		pipeline->cmds[cmd_i].redirs = seg.redirs;
		seg.redirs = NULL;
		if (i < input->n_tokens && tokens[i].type == TOK_PIPE)	// Skip the pipe token
			i++;
		cmd_i++;
	}
	if (i != input->n_tokens)
	{
		// free?
		return (-1);
	}
	pipeline->n_cmds = n_cmds;
	return (0);
}

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

int	append_arg(t_token *tokens, int i, t_seg *seg)
{
	t_list	*node;
	char 	*arg;

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

int	get_redir_type(TokType type, t_redir_type *t_type)
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

int	is_redir_tok(TokType type)
{
	if (type == TOK_HEREDOC || type == TOK_REDIR_IN
			|| type == TOK_REDIR_OUT || type == TOK_REDIR_APP)
		return (1);
	return (0);
}

int	count_pipes(t_input *input)
{
	int count;
	int	i;

	if (!input->tokens)
		return (0);
	count = 0;
	i = 0;
	while (i < input->n_tokens)
	{
		if (input->tokens[i].type == TOK_PIPE)
			count++;
		i++;
	}
	return (count);
}
