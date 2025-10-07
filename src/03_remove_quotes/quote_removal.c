#include "minishell.h"

/**
 * @brief Remove outer quotes from WORD tokens in-place.
 * @param input Input struct (for token array).
 * @return 0 on success, -1 on error or invalid input.
 */
int	remove_quotes(t_input *input)
{
	int		i;
	char	*unquoted_text;

	if (!input || !input->tokens)
		return (-1);
	i = 0;
	while (i < input->n_tokens)
	{
		if (input->tokens[i].type == TOK_WORD)							// Only remove quotes on WORD tokens
		{
			unquoted_text = handle_quote_removal(input->tokens[i].text, &input->tokens[i].was_quoted); // Rebuilds token.text with quotes removed.
			if (!unquoted_text)
				return (-1);
			free(input->tokens[i].text);								// Free the old token.text
			input->tokens[i].text = unquoted_text;						// Replace token.text with unquoted_text
			if (unquoted_text[0] == '\0' && input->tokens[i].was_quoted == 0) // Check if the token.text is an empty string and the string was NOT quoted (empty expansion).
				input->tokens[i].type = TOK_VOID;
		}
		i++;
	}
	return (0);
}

/**
 * @brief Creates a new string with outer quotes stripped.
 * @param text Source string (may be NULL).
 * @return Newly allocated unquoted string, or NULL on error.
 */
char *handle_quote_removal(char *text, int *was_quoted)
{
	char	*unquoted_text;

	if (!text)
		return (ft_strdup(""));
	unquoted_text = ft_strdup("");							// Init unquoted_text as an empty string
	if (!unquoted_text)
		return (NULL);
	if (create_unquoted_text(text, &unquoted_text, was_quoted) == -1)	// Rebuilds the unquoted_text.
	{
		free(unquoted_text);
		return (NULL);
	}
	return (unquoted_text);									// Return the text with quotes removed
}

/**
 * @brief Build unquoted_text by iterating over text and skipping outer quotes.
 * @param text           Source string.
 * @param unquoted_text  Unquoted text buffer.
 * @return 0 on success, -1 on error.
 */
int	create_unquoted_text(char *text, char **unquoted_text, int *was_quoted)
{
	int	i;
	int in_single;
	int	in_double;
	int	quote_removed;

	i = 0;
	in_double = 0;
	in_single = 0;
	while (text[i] != '\0')
	{
		quote_removed = remove_outer_quote(text[i], &in_single, &in_double);	// Checks if the char is a quote char and an "outer quote"
		if (quote_removed == 1)		// 1 == quote removed (not appended to unquoted_text)
		{
			*was_quoted = 1;		// Sets the token->was_quoted flag to true.
			i++;
			continue ;
		}							// 0 ==  not a quote char / nested quote.
		if (append_char(unquoted_text, text[i]) == -1)	// If char was not a quote char - Append it to the unquoted_text string
			return (-1);
		i++;
	}
	return (0);
}

/**
 * @brief If c is an outer quote, toggle state and skip it.
 * @param c         Current character.
 * @param in_single Single-quote state.
 * @param in_double Double-quote state.
 * @return 1 if an outer quote was removed, 0 otherwise.
 */
int	remove_outer_quote(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !*in_double)		// If char is single quote and we are not inside double quotes
	{									// Outer single quote found -> Remove it (aka. do not append it)
		*in_single = !*in_single;		// Toggle the flag.
		return (1);						// Return 1 to signal quote was removed
	}
	if (c == '\"' && !*in_single)		// Same but for ""
	{
		*in_double = !*in_double;
		return (1);
	}
	return 0;							// Return 0 to signal non quote char / inner quote.
}
