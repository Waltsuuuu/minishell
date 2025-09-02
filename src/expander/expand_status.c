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
	t_expand_state st;

	init_expand_state(&st);
	while (text[st.i] != '\0')
	{
		st.quote_handled = process_quote_char(text[st.i], &st.in_single, &st.in_double, exp_text); // Appends quote characters to the expaneded_text string.
		if (st.quote_handled == -1)
			return (-1);
		if (st.quote_handled == 1)
		{
			st.i++;
			continue ;
		}
		if (!st.in_single && text[st.i] == '$' && text[st.i + 1] == '?')		// If not inside single quotes and status marker ($?) found,
		{
			if (process_expanded_str(exp_text, status_str) == -1)		// Append the status string to the expanded_text string.
				return (-1);
			st.i += 2;													// Skips over the $ and ? chars
			continue ;
		}
		if (process_char(exp_text, text[st.i]) == -1)					// Append normal char to expanded_text string.
			return (-1);
		st.i++;
	}
	return (0);
}
