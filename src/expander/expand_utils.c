#include "minishell.h"

int	valid_cont_char(char c)
{
	if (ft_isalpha(c) || (c == '_') || ft_isdigit(c))
		return (1);
	return (0);
}

int	valid_start_char(char c)
{
	if (ft_isalpha(c) == 1024|| (c == '_'))
		return (1);
	return (0);
}

// Appends the status string to the current exp_text
int	process_expanded_str(char **exp_text, const char *status_str)
{
	char	*joined;

	if (!exp_text || !*exp_text || !status_str)
		return (-1);
	joined = ft_strjoin(*exp_text, status_str);
	if (!joined)
		return (-1);
	free(*exp_text);
	*exp_text = joined;
	return (0);
}

// Appends quote character to the current exp_text AND keeps track if current chars are inside quotes.
int	process_quote_char(char c, int *in_single, int *in_double, char **exp_text)
{
	if (c == '\'' && !*in_double)				// If single quote and not already inside double quotes
	{
		*in_single = !*in_single;				// Flip quote switch.
		if (process_char(exp_text, '\'') == -1)	// Adds quote char to expanded_text string.
			return (-1);
		return (1);
	}
	if (c == '\"' && !*in_single)				// If double quote and not already inside single quotes
	{
		*in_double = !*in_double;				// Flip quote switch.
		if (process_char(exp_text, '\"') == -1)	// Adds quote char to expanded_text string.
			return (-1);
		return (1);
	}
	return (0);
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
