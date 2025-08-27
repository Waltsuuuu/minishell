#include "minishell.h"

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
int	init_tokenizer_state(t_tokenizer_state *state, const char *line)
{
	if (!state)
		return (-1);
	state->line = line;
	state->index = 0;
	state->in_double = 0;
	state->in_single = 0;
	state->word_start = -1;	// -1 means no active word yet.
	state->tokens = NULL;
	state->n_tokens = 0;
	state->head = NULL;
	state->tail = NULL;
	state->list_count = 0;
	return (0);
}

int	clear_tokenizer_state(t_tokenizer_state *state)
{
	if (!state)
		return (-1);
	if (state->tokens)
		free_token_array(state);
	if (state->head)
		free_token_list(state);
	state->tokens = NULL;
	state->n_tokens = 0;
	state->index = 0;
	state->in_double = 0;
	state->in_single = 0;
	state->word_start = -1;
	state->head = NULL;
	state->tail = NULL;
	state->list_count = 0;
	return (0);
}

int	free_token_array(t_tokenizer_state *state)
{
	int	i;

	if (!state)
		return (-1);
	i = 0;
	while (i < state->n_tokens)
	{
		if (state->tokens[i].text)
			free(state->tokens[i].text);
		i++;
	}
	free(state->tokens);
	return (0);
}

int	free_token_list(t_tokenizer_state *state)
{
	t_toknode	*current;
	t_toknode	*next;

	if (!state)
		return (-1);
	current = state->head;
	while (current)
	{
		next = current->next;
		if (current->tok.text)
			free(current->tok.text);
		free(current);
		current = next;
	}
	return (0);
}

/**
 * Runs the tokenizer over the current input line.
 *
 * Iterates through the input line. When not inside quotes,
 * skips blanks, then detects operators (|, <, >, <<, >>) and calls
 * set_operator(), otherwise calls set_word(). Inside quotes, continues
 * the current word with set_word(). Each helper adds a token and advances
 * state->index.
 *
 * @param state  Tokenizer state (must have line initialized).
 * @return 0 on success, -1 on error (e.g., allocation failure in helpers).
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

int	skip_blanks(const char *string, int i)
{
	if (!string)
		return (i);
	while (string[i] == ' ' || string[i] == '\t')
		i++;
	return (i);
}

int	get_operator_len(const char *string)
{
	if (!string || !*string)
		return (0);
	if (*string == '|')
		return (1);
	if (*string == '<')
	{
		if (string[1] == '<')
			return (2);
		return (1);
	}
	if (*string == '>')
	{
		if (string[1] == '>')
			return (2);
		return (1);
	}
	return (0);
}

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

TokType	get_operator_type(const char *string, int len)
{
	if (len == 1)
	{
		if (string[0] == '|')
			return (TOK_PIPE);
		if (string[0] == '<')
			return (TOK_REDIR_IN);
		if (string[0] == '>')
			return (TOK_REDIR_OUT);	
	}
	else if (len == 2)
	{
		if (string[0] == '<' && string[1] == '<')
			return (TOK_HEREDOC);
		if (string[0] == '>' && string[1] == '>')
			return (TOK_REDIR_APP);
	}
	return (TOK_WORD);
}

int	set_word(t_tokenizer_state *state)
{
	char	*text;
	int		len;

	if (!state)
		return (-1);
	if (state->word_start == -1)
		state->word_start = state->index;								// Mark the start index of the word.
	state->index = scan_word_end(state->line, state->index,				// Find the end of the word and increase index to the end.
				&state->in_single, &state->in_double);
	len = state->index - state->word_start;								// Calc the len of the word (end - start)
	text = ft_substr(state->line, state->word_start, len);				// Extract the word.
	if (!text)
		return (-1);
	if (append_token_node(state, TOK_WORD, text, state->word_start) == -1)	// Add token to the linked list
		return (-1);
	state->word_start = -1;												// Reset word_start to -1 (no active word)
	return (0);
}

int	scan_word_end(const char *string, int i, int *in_single, int *in_double)
{
	if (!string)
		return (i);
	while (string[i] != '\0')											// Loop over 
	{
		if (!*in_single && !*in_double)									// When NOT inside quotes
		{
			if (string[i] == ' ' || string[i] == '\t'
				|| get_operator_len(&string[i]) > 0)	// If the current char is space OR operator char is found
				break ;													// 	- En of word / Operator found
			if (string[i] == '\'') 										// If the current char is a single quote opener '
				*in_single = 1;											//	- Set the in_single flag to true
			if (string[i] == '\"')										// If the current char is a douvle quote opener "
				*in_double = 1;											//  - Set the in_double flag to true
			i++;														// Move to next char
		}
		else															// When INSIDE quotes
		{
			if (*in_single && string[i] == '\'')						// If single quote closer char found
				*in_single = 0;											// 	- Set the in_singe flag to false
			else if (*in_double && string[i] == '\"')					// If double quote closer char found
				*in_double = 0;											// 	- Set the in_double flag to false
			i++;														// Move to next char
		}
	}
	return (i);															// Return the index just after the end of the word
}

int	append_token_node(t_tokenizer_state *state, TokType type, char *text, int pos)
{
	t_toknode	*node;

	if (!state || !text)
		return (-1);
	node = malloc(sizeof(t_toknode));	// Malloc the node
	if (!node)
	{
		free(text);
		return (-1);
	}
	node->tok.type = type;			// Set the type.
	node->tok.text = text;			// Set the text (takes ownership of the malloced string), now in charge of freeing it.
	node->tok.pos = pos;			// Set the index in the raw string.
	node->next = NULL;				// Current node becomes the tail, so node->next = NULL.
	if (!state->head)				// If there is no head node.
		state->head = node;			// 	- Set the node as the head.
	else
		state->tail->next = node;	//	Else - link the current tail to the new node 
	state->tail = node;				// Then set the current node as the tail
	state->list_count += 1;			// Increase the node list count
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
