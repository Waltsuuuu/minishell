/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:48:59 by wheino            #+#    #+#             */
/*   Updated: 2025/10/09 11:30:50 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * CREATE TOKENS OUT OF INPUT STRING.
 * Tokens are delimited by spaces or tabs,
 * except inside single or double quotes.
 * Operators |, <, >, <<, >> always form their own separate tokens.
 * @return 0 on success, -1 on error. 
 */
int	tokenize_line(const char *line, t_input *input)
{
	t_tokenizer_state	state;

	if (!line || !input)
		return (-1);
	input->tokens = NULL;
	input->n_tokens = 0;
	if (init_tokenizer_state(&state, line) == -1)
		return (-1);
	if (run_tokenizer(&state) == -1)
	{
		clear_tokenizer_state(&state);
		return (-1);
	}
	if (convert_lst_to_arr(&state) == -1)
	{
		clear_tokenizer_state(&state);
		return (-1);
	}
	state.head = NULL;
	state.tail = NULL;
	state.list_count = 0;
	input->tokens = state.tokens;
	input->n_tokens = state.n_tokens;
	return (0);
}

/**
 * Iterates through the input line.
 * Skips whitespace when not inside quotes.
 * Checks if current character is the start of an operator segment.
 * If true - calls set_operator(),
 * Else - calls set_word().
 * Helpers move iterator forward as needed.
 * @return 0 on success, -1 on error (malloc failure in helpers). 
 */
int	run_tokenizer(t_tokenizer_state *state)
{
	if (!state)
		return (-1);
	while (state->line[state->index] != '\0')
	{
		state->index = skip_blanks(state->line, state->index);
		if (state->line[state->index] == '\0')
			break ;
		if (get_operator_len(&state->line[state->index]) > 0)
		{
			if (set_operator(state) == -1)
				return (-1);
		}
		else
			if (set_word(state) == -1)
				return (-1);
	}
	return (0);
}

/**
 * CREATES OPERATOR TOKEN TOKEN ( |, <, >, >>, << ),
 * appends the created token to a temporary linked list of tokens.
 * @return 0 on sucess, -1 on memory allocation failure.
 */
int	set_operator(t_tokenizer_state *state)
{
	int			len;
	char		*text;
	e_tok_type	type;

	if (!state)
		return (-1);
	len = get_operator_len(&state->line[state->index]);
	text = ft_substr(state->line, state->index, len);
	if (!text)
		return (-1);
	type = get_operator_type(&state->line[state->index], len);
	if (append_token_node(state, type, text, state->index) == -1)
		return (-1);
	state->index += len;
	return (0);
}

/**
 * CREATES WORD TOKEN out of non-operator segment,
 * appends the created token to a temporary linked list of tokens.
 * @return 0 on sucess, -1 on memory allocation failure. 
 */
int	set_word(t_tokenizer_state *state)
{
	char	*text;
	int		len;

	if (!state)
		return (-1);
	if (state->word_start == -1)
		state->word_start = state->index;
	state->index = scan_word_end(state->line, state->index,
			&state->in_single, &state->in_double);
	len = state->index - state->word_start;
	text = ft_substr(state->line, state->word_start, len);
	if (!text)
		return (-1);
	if (append_token_node(state, TOK_WORD, text, state->word_start) == -1)
		return (-1);
	state->word_start = -1;
	return (0);
}

/**
 * CONVERTS TEMPORARY LINKED LIST OF TOKENS INTO AN ARRAY.
 * Copies each nodes token into state->tokens,
 * frees the list nodes, and sets state->n_tokens accordingly.
 * If the list is empty, leaves state->tokens as NULL and returns success.
 * @return 0 on success (including empty list),
 * @return -1 on memory allocation failure. 
 */
int	convert_lst_to_arr(t_tokenizer_state *state)
{
	t_toknode	*current;
	t_toknode	*next;
	int			i;

	if (!state)
		return (-1);
	state->n_tokens = state->list_count;
	if (state->n_tokens == 0)
		return (0);
	state->tokens = malloc(sizeof(t_token) * state->n_tokens);
	if (!state->tokens)
		return (-1);
	current = state->head;
	i = 0;
	while (current)
	{
		state->tokens[i] = current->tok;
		next = current->next;
		free(current);
		current = next;
		i++;
	}
	return (0);
}
