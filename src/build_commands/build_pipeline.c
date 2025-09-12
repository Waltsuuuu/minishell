#include "minishell.h"

/**
 * Parses tokens into structured commands (executable pipeline).
 * Modifies t_pipeline *pipeline in place.
 * @return 0 on success, -1 on syntax/alloc error.
 */
int	build_pipeline(t_input *input, t_token *tokens, t_pipeline *pipeline)
{
	int		n_cmds;
	int		i;
	int		cmd_i;
	t_seg	seg;

	n_cmds = bp_prepare(input, tokens, pipeline);
	if (n_cmds <= 0)
		return (n_cmds);
	pipeline->n_cmds = n_cmds;
	i = 0;
	cmd_i = 0;
	while (cmd_i < n_cmds)
	{
		bp_seg_init(&seg, i);
		if (bp_fill_segment(input, tokens, &seg, &i) == -1)
			return (err_exit_build_pipeline(pipeline, &seg, cmd_i));
		if (bp_finalize_command(&seg, pipeline, cmd_i, tokens, i) == -1)
			return (err_exit_build_pipeline(pipeline, &seg, cmd_i));
		if (i < input->n_tokens && tokens[i].type == TOK_PIPE)
			i++;
		cmd_i++;
	}
	if (i != input->n_tokens)
		return (err_exit_build_pipeline(pipeline, &seg, cmd_i));
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
				return(-1);
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

int	bp_finalize_command(t_seg *seg, t_pipeline *pipeline, int cmd_i, t_token *tokens, int i)
{
	if (seg->argc == 0 && seg->redirs == NULL)		// Empty command
	{
		if (cmd_i + 1 == pipeline->n_cmds)			// After trailing pipe
			printf("Syntax error near newline\n");
		else										// Between pipes
			printf("Syntax error near '%s' at position %d\n", tokens[i].text, tokens[i].pos);
		return (-1);
	}
	if (arg_ll_to_arr(seg, pipeline, cmd_i) == -1)
		return (-1);
	pipeline->cmds[cmd_i].argc = seg->argc;
	pipeline->cmds[cmd_i].redirs = seg->redirs;
	seg->redirs = NULL;
	return (0);
}

