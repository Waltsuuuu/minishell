#include "minishell.h"

/**
 * CREATE TOKENS OUT OF INPUT STRING.
 * 
 * @param line Input line (quotes must be balanced)
 * @param out Pointer to t_token array. Will be modified in place.
 * @param out_count Pointer to n_tokens counter. Will be modified in place.
 * @return 0 on success, -1 on error (frees memory that was allocated inside function). 
 */
int	tokenize_line(const char *line, t_token **out, int *out_count)
{
	t_tokenizer_state state;

	if (!line || !out || !out_count)
		return (-1);
	*out = NULL;
	*out_count = 0;
	if (init_tokenizer_state(&state, line) == -1)	// Sets the tokenizer variables to 0 / NULL.
		return (-1);
	if (run_tokenizer(&state) == -1)				//	Extracts the tokens from the raw line, stores the tokens in a temp linked list
	{
		clear_tokenizer_state(&state);
		return (-1);
	}
	if (convert_lst_to_arr(&state) == -1)			// Converst the linked list to array of tokens
	{
		clear_tokenizer_state(&state);
		return (-1);
	}
	state.head = NULL;
	state.tail = NULL;
	state.list_count = 0;
	*out = state.tokens;							// Sets the t_token array of t_input
	*out_count = state.n_tokens;					// Sets the token count 
	return (0);
}

/**
 * Iterates through the input line. When NOT inside quotes,
 * skips blanks, then detects operators (|, <, >, <<, >>) and calls
 * set_operator(), otherwise calls set_word(). When INSIDE quotes, creates a WORD token. 
 *
 * @param state  Tokenizer state (must have line initialized).
 * @return 0 on success, -1 on error (malloc failure in helpers). 
 */
int	run_tokenizer(t_tokenizer_state *state)
{
	if (!state)
		return (-1);
	while (state->line[state->index] != '\0')						// While there are characters to be checked			
	{
		if (!state->in_single && !state->in_double)					// If NOT inside quotes...
		{
			state->index = skip_blanks(state->line, state->index);	// 	Skip spaces
			if (state->line[state->index] == '\0')					// 	End of line check after skipping spaces.
				break ;												//		- If end, exit loop.
			if (get_operator_len(&state->line[state->index]) > 0)	// 	Checks for if we are dealing with a token (1 or 2 = operator match found, 0 = not an operator)
			{
				if (set_operator(state) == -1)						// 		-If operator, create operator token.
					return (-1);
			}
			else if (set_word(state) == -1 )						// 		-If not operator, create word token.
				return (-1);
		}
		else														// If inside quotes...
		{
			if (set_word(state) == -1)								// Create word token.
				return (-1);
		}
	}
	return (0);
}

/**
 * CREATES OPERATOR TOKEN TOKEN ( |, <, >, >>, << ) and appends t_toknode to temporary linked list of tokens.
 * @param state Tokenizer state.
 * 
 * @return 0 on sucess.
 */
int	set_operator(t_tokenizer_state *state)
{
	int		len;
	char	*text;
	TokType	type;

	if (!state)
		return (-1);
	len = get_operator_len(&state->line[state->index]);				// Find length of the operator - Either 1 or 2 depending on the operator.
	if (len <= 0)
		return (-1);
	text = ft_substr(state->line, state->index, len);				// Extract the operator string.
	if (!text)
		return (-1);
	type = get_operator_type(&state->line[state->index], len);		// Find and set the operator type
	if (append_token_node(state, type, text, state->index) == -1)	// Adds token to the temp linekd list of tokens
		return (-1);
	state->index += len;											// Advance past the operator
	return (0);														// Return success
}
/**
 * CREATES WORD TOKEN out of non-operator string and appends t_toknode to temporary linked list of tokens.
 * @param state Tokenizer state.
 * 
 * @return 0 on sucess.
 */
int	set_word(t_tokenizer_state *state)
{
	char	*text;
	int		len;

	if (!state)
		return (-1);
	if (state->word_start == -1)
		state->word_start = state->index;									// Mark the start index of the word.
	state->index = scan_word_end(state->line, state->index,					// Find the end of the word and increase index to the end.
				&state->in_single, &state->in_double);
	len = state->index - state->word_start;									// Calc the len of the word (end - start)
	text = ft_substr(state->line, state->word_start, len);					// Extract the word.
	if (!text)
		return (-1);
	if (append_token_node(state, TOK_WORD, text, state->word_start) == -1)	// Add token to the linked list
		return (-1);
	state->word_start = -1;													// Reset word_start to -1 (no active word)
	return (0);
}


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
