#include "minishell.h"

/**
 * @brief Expand $VAR in text using envp. 
 * @param text  Input string (may be NULL).
 * @param envp  Environment variable array.
 * @return Newly allocated expanded string, or NULL on error.
 */
char *expand_variable(char *text, char **envp)
{
	char	*exp_text;

	if (!text)
		return (ft_strdup(""));
	exp_text = ft_strdup("");
	if (!exp_text)
		return (NULL);
	if (create_exp_var_text(text, &exp_text, envp) == -1)
	{
		free(exp_text);
		return (NULL);
	} 
	return (exp_text);
}

/**
 * @brief Rebuilds token.text, expands $VAR if found. 
 * @param text     Source string to scan.
 * @param exp_text Destination string pointer.
 * @param envp     Environment variable array.
 * @return 0 on success, -1 on error.
 */
int	create_exp_var_text(char *text, char **exp_text, char **envp)
{
	t_expand_state st;

	init_expand_state(&st);													// Inits iterator + in_single and in_double quote flags.
	while (text[st.i] != '\0')
	{
		st.quote_handled = process_quote_char(text[st.i], &st.in_single,	// Appends quote char to exp_text string.
				&st.in_double, exp_text);									// And keeps track of quote state.
		if (st.quote_handled == -1)											// -1 == Failure.
			return (-1);
		if (st.quote_handled == 1)											// 1 == quote char found and handled.
		{
			st.i++;
			continue ;
		}
		st.expanded = handle_var_expansion(text, exp_text, &st.i,			// Checks if current char is the beginning of a $VAR to expand
				envp, st.in_single);
		if (st.expanded == -1)												// -1 == Failure
			return (-1);
		if (st.expanded == 1)												// 1 if found something to expand.
			continue ;
		if (process_char(exp_text, text[st.i]) == -1)						// If not a quote or var, append char to exp_text string.
			return (-1);
		st.i++;																// Move to next char.
	}
	return (0);
}

/**
 * @brief If at a $VAR start (and not in '), expand it via process_var_expansion.
 * @return 1 if expanded, 0 if not applicable, -1 on error.
 */
int	handle_var_expansion(char *text, char **exp_text, int *i, char **envp, int in_single)
{
	int	status;

	status = 0;
	if (!in_single && text[*i] == '$' && valid_start_char(text[*i + 1]) == 1)
	{
		(*i)++;
		status = process_var_expansion(text, exp_text, i, envp);
		if (status == -1)
			return (-1);
		return (1);
	}
	return (0);
}

/**
 * @brief Read KEY after '$' and append its env value if found.
 * @return 1 if expanded, 0 if missing in env, -1 on error.
 */
int	process_var_expansion(char *text, char **exp_text, int *i, char **envp)
{
 	char	*key;															// "KEY=" buffer ex. USER= or PATH=
 	int		key_len;														// length of key
 	int		start_i;														// Index where KEY starts in text string.
 	int		env_i;															// Iterator for envp array.

	key = NULL;
	if (extract_key(text, i, &key, &key_len, &start_i) == -1)				// Parses the KEY from text.
		return (-1);
	env_i = find_env_index(envp, key, key_len);								// Searches for matching KEY in envp array and returns its index.
	if (env_i >= 0)															// -1 if no matching KEY found, > 0 if matching KEY found.
	{
		if (process_expanded_str(exp_text, &envp[env_i][key_len + 1]) == -1)	// Appends the VALUE section of the KEY=VALUE string to exp_text.
		{																		// -1  == Failure.
			free(key);
			return (-1);
		}
		free(key);
		return (1);																// Return 1 to signal successful VAR expansion and append OK.
	}
	free(key);
	return (0);																	// 0 if VAR not found.
}

