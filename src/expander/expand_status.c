#include "minishell.h"

char *expand_status(char *text, int last_status)
{
	char	*exp_text;
	char	*status_str;

	if (!text)
		return (ft_strdup(""));
	exp_text = ft_strdup("");										// Make exp_text a valid string.
	if (!exp_text)
		return (NULL);
	status_str = ft_itoa(last_status);								// Convert int (last_status) to string
	if (!status_str)
	{
		free(exp_text);
		return (NULL);
	}
	if (create_exp_status_text(text, &exp_text, status_str) == -1)  // Builds the expaneded text string
	{
		free(status_str);
		free(exp_text);
		return (NULL);
	}
	free(status_str);												// Free the status string		
	return (exp_text);												// Return the token.text in expanded form
}

// Rebuilds token.text character by character, if $? is found, it is replaced by the status number.  
int	create_exp_status_text(char *text, char **exp_text, char *status_str)
{
	int	i;
	int in_single;
	int	in_double;
	int quote_handled;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (text[i] != '\0')
	{
		quote_handled = process_quote_char(text[i], &in_single, &in_double, exp_text); // Appends quote characters to the expaneded_text string.
		if (quote_handled == -1)
			return (-1);
		if (quote_handled == 1)
		{
			i++;
			continue ;
		}
		if (!in_single && text[i] == '$' && text[i + 1] == '?')		// If not inside single quotes and status marker ($?) found,
		{
			if (process_expanded_str(exp_text, status_str) == -1)		// Append the status string to the expanded_text string.
				return (-1);
			i += 2;													// Skips over the $ and ? chars
			continue ;
		}
		if (process_char(exp_text, text[i]) == -1)					// Append normal char to expanded_text string.
			return (-1);
		i++;
	}
	return (0);
}