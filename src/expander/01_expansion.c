#include "minishell.h"

/**
 * @brief Expand WORD tokens for exit status ($?) and environment ($VAR) variables.
 *
 * @param input        Tokenized command input; modified in place.
 * @param last_status  Last command exit status used for "$?" expansion.
 * @param envp         Null-terminated environment array used for "$VAR" expansion.
 *
 * @return 0 on success; -1 on allocation/expansion error.
 *
 */
int expand_tokens(t_input *input, int last_status, char **envp)
{
	int			i;
	char		*exp_text;

	if (!input || !input->tokens)
		return (0);
	i = 0;
	while (i < input->n_tokens)												// Loop through all tokens
	{
		if ((input->tokens[i].type == TOK_WORD)								// Only WORD tokens are expanded	
				&& (i == 0 || input->tokens[i - 1].type != TOK_HEREDOC))	// WORD tokens preceeded by HEREDOC tokens are not expanded. (heredoc delimiter)
		{
			exp_text = expand_status(input->tokens[i].text, last_status);	// Rebuilds token.text with the status expanded.
			if (!exp_text)
				return (-1);
			free(input->tokens[i].text);									// Free the old token.text
			input->tokens[i].text = exp_text;								// Replace token.text with expanded text
			exp_text = expand_variable(input->tokens[i].text, envp);		// Rebuilds token.text with variable ($VAR) expanded.
			if (!exp_text)
				return (-1);
			free(input->tokens[i].text);									// Free the old token.text
			input->tokens[i].text = exp_text;								// Replace token.text with expanded text
		}
		i++;																// Move to next token.
	}
	return (0);																// Return success.
}
