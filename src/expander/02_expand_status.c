#include "minishell.h"

/**
 * @brief Expand "$?" using last_status into a new string.
 * @param text         Input string (can be NULL).
 * @param last_status  Exit status used for "$?".
 * @return Newly allocated expanded string, or NULL on error.
 * @note Expansion is skipped inside single quotes; quote chars are preserved.
 */
char *expand_status(char *text, int last_status)
{
	char	*exp_text;
	char	*status_str;

	if (!text)														// If text == NULL
		return (ft_strdup(""));										// Return empty string (Nothing to be expanded)
	exp_text = ft_strdup("");										// Init exp_text as an empty string.
	if (!exp_text)
		return (NULL);
	status_str = ft_itoa(last_status);								// Convert int (last_status) to string
	if (!status_str)
	{
		free(exp_text);
		return (NULL);
	}
	if (create_exp_status_text(text, &exp_text, status_str) == -1)  // Builds the expaneded token.text string
	{
		free(status_str);
		free(exp_text);
		return (NULL);
	}
	free(status_str);												// Free the status string		
	return (exp_text);												// Return the token.text in expanded form
}

/**
 * @brief Replace "$?" in text with status_str while preserving quote state.
 * @param text       Source string to scan.
 * @param exp_text   [in,out] Destination string pointer.
 * @param status_str Status string to insert.
 * @return 0 on success, -1 on error.
 * @note Skips expansion inside single quotes; appends quote chars as seen.
 */
int	create_exp_status_text(char *text, char **exp_text, char *status_str)
{
	t_expand_state st;

	init_expand_state(&st);													// Inits iterator + in_single and in_double quote flags.
	while (text[st.i] != '\0')
	{
		st.quote_handled = process_quote_char(text[st.i], &st.in_single,	// Appends quote char to the exp_text string.
			 &st.in_double, exp_text); 										// And keeps track of quote state. 
		if (st.quote_handled == -1)											// -1 == Failure.
			return (-1);
		if (st.quote_handled == 1)											// 1 == quote char found and handled.
		{																	
			st.i++;
			continue ;
		}																    // 0 == current char is not a quote -> move to next checks.		
		if (!st.in_single && text[st.i] == '$' && text[st.i + 1] == '?')	// If status marker ($?) found, and not inside single quotes.
		{
			if (process_expanded_str(exp_text, status_str) == -1)			// Append the status string to the exp_text string.
				return (-1);
			st.i += 2;														// Skips over the $ and ? chars.
			continue ;
		}
		if (process_char(exp_text, text[st.i]) == -1)						// Append any other char to exp_text string.
			return (-1);
		st.i++;																// Move to next char.
	}
	return (0);
}
