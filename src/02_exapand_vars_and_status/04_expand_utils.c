#include "minishell.h"

int	valid_cont_char(char c)
{
	if (ft_isalpha(c) || (c == '_') || ft_isdigit(c))
		return (1);
	return (0);
}

int	valid_start_char(char c)
{
	if (ft_isalpha(c) || (c == '_'))
		return (1);
	return (0);
}

/**
 * @brief Append the status_string to exp_text.
 * @param exp_text Destination string pointer.
 * @param str String to append.
 * @return 0 on success, -1 on error.
 */
int	process_expanded_str(char **exp_text, const char *str)
{
	char	*joined;

	if (!exp_text || !*exp_text || !str)
		return (-1);
	joined = ft_strjoin(*exp_text, str);
	if (!joined)
		return (-1);
	free(*exp_text);
	*exp_text = joined;
	return (0);
}

/**
 * @brief Handle quote character: toggle quote state and append the quote.
 * @param c Current input char.
 * @param in_single Single-quote state (1 if inside, 0 if not).
 * @param in_double Double-quote state (1 if inside, o if not).
 * @param exp_text  Expanded text buffer.
 * @return 1 if quote handled, 0 if not a quote, -1 on error.
 */
int	process_quote_char(char c, int *in_single, int *in_double, char **exp_text)
{
	if (c == '\'' && !*in_double)				// If single quote char outside double quotes.
	{
		*in_single = !*in_single;				// Toggle quote flag.
		if (process_char(exp_text, '\'') == -1)	// Append ' to exp_text.
			return (-1);
		return (1);								// Return 1 to signal input was a quote and append OK
	}
	if (c == '\"' && !*in_single)				// If double quote and outside single quotes
	{
		*in_double = !*in_double;				// Flip quote switch.
		if (process_char(exp_text, '\"') == -1)	// Append " to expanded_text string.
			return (-1);	
		return (1);								// Return 1 to signal input was a quote and append OK
	}
	return (0);									// Return 0 if char was not a quote.
}

// Appends a single char to the current exp_text
int	process_char(char **exp_text, char c)
{
	char 	buf[2];
	char	*joined;

	if (!exp_text || !*exp_text)
		return (-1);
	buf[0] = c;
	buf[1] = '\0';
	joined = ft_strjoin(*exp_text, buf);
	if (!joined)
		return (-1);
	free(*exp_text);
	*exp_text = joined;
	return (0);
}
