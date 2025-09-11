#include "minishell.h"

// Refactor plan
// Split build_pipeline() into 4 helpers
// 1. Prepare        - Setup pipeline, alloc cmds, handle empty/leading pipe
// 2. Init segment   - Reset t_seg fields before building next command
// 3. Fill segment   - Walk tokens until '|' and build argv/redirs
// 4. Finalize cmd   - Transfer seg data into pipeline->cmds

int	build_pipeline(t_input *input, t_token *tokens, t_pipeline *pipeline)
{
	int		n_cmds;
	int		i;
	int		cmd_i;
	t_seg	seg;

	n_cmds = bp_prepare(input, tokens, pipeline);
	if (n_cmds <= 0)
		return (n_cmds);
	i = 0;
	cmd_i = 0;
	while (cmd_i < n_cmds)
	{
		bp_seg_init(&seg, i);
		if (bp_fill_segment(input, tokens, &seg, &i) == -1)
			return (err_exit_build_pipeline(pipeline, &seg, cmd_i));
		if (bp_finalize_command(&seg, pipeline, cmd_i) == -1)
			return (err_exit_build_pipeline(pipeline, &seg, cmd_i));
		if (i < input->n_tokens && tokens[i].type == TOK_PIPE)
			i++;
		cmd_i++;
	}
	if (i != input->n_tokens)
		return (err_exit_build_pipeline(pipeline, &seg, cmd_i));
	pipeline->n_cmds = n_cmds;
	return (0);
}

/**
 * Prepare pipeline: Basic syntax checks and alloc pipeline->cmds array.
 * @return -1 on error, 0 if no tokens, >0 = n_cmds allocated. 
 */
int	bp_prepare(t_input *input, t_token *tokens, t_pipeline *pipeline)
{
	int	n_cmds;

	if (!input || !pipeline)
		return (-1);
	if (!tokens || input->n_tokens == 0)
	{
		pipeline->n_cmds = 0;
		pipeline->cmds = NULL;
		return (0);
	}
	n_cmds = count_pipes(input) + 1;
	pipeline->cmds = ft_calloc(n_cmds, sizeof(t_command));
	if (!pipeline->cmds)
		return (-1);
	if (tokens[0].type == TOK_PIPE)
	{
		printf("Syntax error near '%s' at position %d\n", tokens[0].text, tokens[0].pos); // These have to go to stderr i think? Update it later...
		free(pipeline->cmds);
		pipeline->cmds = NULL;
		pipeline->n_cmds = 0;
		return (-1);
	}
	return (n_cmds);
}

void	bp_seg_init(t_seg *seg, int start)
{
	seg->argc = 0;
	seg->start = start;
	seg->args = NULL;
	seg->redirs = NULL;
}

/**
 * Fill segment: consumes tokens until '|' and build argv/redirs.
 * Advances *i, appends to seg->args/redirs, increments seg->argc.
 * @return 0 on success, -1 on syntax/alloc error.
 */
int	bp_fill_segment(t_input *input, t_token *tokens, t_seg *seg, int *i)
{
	while (*i < input->n_tokens && tokens[*i].type != TOK_PIPE)
	{
		if (is_redir_tok(tokens[*i].type))
		{
			if ((*i + 1 >= input->n_tokens) || tokens[*i + 1].type != TOK_WORD)
			{
				printf("Syntax error near '%s' at position %d\n", tokens[*i].text, tokens[*i].pos);
				return(1);
			}
			if (build_and_append_redir(tokens, *i, seg) == -1)
				return (-1);
			*i += 2; // (2 becuase handled redir + target)	
		}
		else if (tokens[*i].type == TOK_WORD)
		{
			if (append_arg(tokens, *i, seg) == -1)
				return (-1);
			seg->argc++;
			*i += 1;
		}
		else
			return (-1);
	}
	return (0);
}

int	bp_finalize_command(t_seg *seg, t_pipeline *pipeline, int cmd_i)
{
	if (arg_ll_to_arr(seg, pipeline, cmd_i) == -1)
		return (-1);
	pipeline->cmds[cmd_i].argc = seg->argc;
	pipeline->cmds[cmd_i].redirs = seg->redirs;
	seg->redirs = NULL;
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
