#include "minishell.h"

/**
 * Finds to the end of a word, respecting quotes. Whitespace or 
 * 
 * @param string Input string.
 * @param i Start index.
 * @param in_single Single quote flag.
 * @param in_double Double quote flag.
 * @return Index after word end.
 */
int	scan_word_end(const char *string, int i, int *in_single, int *in_double)
{
	if (!string)
		return (i);
	while (string[i] != '\0')											// Loop over 
	{
		if (!*in_single && !*in_double)									// When NOT inside quotes
		{
			if (string[i] == ' ' || string[i] == '\t'
				|| get_operator_len(&string[i]) > 0)					// If the current char is space OR operator char is found
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

/**
 * Returns the length of the passed in operator string. If no operator found, returns 0.
 * CAN ALSO BE USED TO CHECK IF STRING POINTS TO START OF OPERATOR CHAR.
 *
 * @param string Pointer to a character.
 * @return 1 or 2 if operator was found. 0 if no operator found. 
 */
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
	node->tok.was_quoted = 0;		// set to default. Real value will be set during quote removal
	node->next = NULL;				// Current node becomes the tail, so node->next = NULL.
	if (!state->head)				// If there is no head node.
		state->head = node;			// 	- Set the node as the head.
	else
		state->tail->next = node;	//	Else - link the current tail to the new node 
	state->tail = node;				// Then set the current node as the tail
	state->list_count += 1;			// Increase the node list count
	return (0);
}

/**
 * Returns the enum TokType of the passed in operator string.
 * @param string Pointer to start of operator in raw string.
 * @param len Calculated length of the operator.
 * 
 * @return Operator type as TokType enum.
 */
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

int	skip_blanks(const char *string, int i)
{
	if (!string)
		return (i);
	while (string[i] == ' ' || string[i] == '\t')
		i++;
	return (i);
}