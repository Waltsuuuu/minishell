#include "minishell.h"

// Only handle $? expansion.
// Does not expand if inside single quotes.
// Quotes are preserved, removal later.
int expand_tokens(t_input *input, int last_status, char **envp)
{
	int			i;
	char		*exp_text;

	if (!input || !input->tokens)
		return (0);
	i = 0;
	while (i < input->n_tokens)												// Loop through all tokens
	{
		if (input->tokens[i].type == TOK_WORD)								// Only WORD tokens are expanded
		{
			exp_text = expand_status(input->tokens[i].text, last_status);	// Builds a new token.text where $? (exit status) is expaneded
			if (!exp_text)
				return (-1);
			free(input->tokens[i].text);
			input->tokens[i].text = exp_text;								// Replace token text with expanded text
			exp_text = expand_variable(input->tokens[i].text, envp);		// Expands the $VAR
			if (!exp_text)
				return (-1);
			free(input->tokens[i].text);
			input->tokens[i].text = exp_text;	
		}
		i++;
	}
	return (0);
}

char *expand_variable(char *text, char **envp)
{
	char	*exp_text;
	int		i;

	if (!text)
		return (ft_strup(""));
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

int	create_exp_var_text(char *text, char **exp_text, char **envp)
{
	int	i;
	int	in_single;
	int	in_double;
	int	quote_handled;

	i = 0;
	in_single = 0;
	in_double = 0;
	while (text[i] != '\0')
	{
		quote_handled = process_quote_char(text[i], &in_single, &in_double, exp_text);
		if (quote_handled == -1)
			return (-1);
		if (quote_handled == 1)
		{
			i++;
			continue ;
		}
		if (!in_single && text[i] == '$' && valid_start_char(text[i]) == 1)
		{
			i++;
			if (process_var_expansion(text, exp_text, &i, envp) == -1)
				return (-1);
			continue ;
		}
		if (process_char(exp_text, text[i]) == 1)
			return (-1);
		i++;
	}
	return (0);
}

int	process_var_expansion(char *text, char **exp_text, int *i, char **envp)
{
	char	*var_name;
	int		start_i;
	int		var_name_len;
	int		env_i;
	char	*expanded_var;

	start_i = *i;
	var_name_len = 0;
	var_name = ft_strdup("");
	if (!var_name)
		return (-1);
	while (valid_cont_char(text[*i]) == 1)
	{
		i++;
		var_name_len++;
	}
	var_name = malloc(var_name_len + 2);
	ft_strlcpy(var_name, &text[start_i], var_name_len);
	var_name[var_name_len] = '=';
	var_name[var_name_len + 1] = '\0';
	while (envp[env_i])
	{
		if (ft_strncmp(envp[env_i], var_name, var_name_len) == 0)
		{
			expanded_var = malloc(ft_strlen(envp[env_i]) - (var_name_len + 1));
			
		}
	}
	// continue....
}

int	valid_cont_char(char c)
{
	if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || (c == '_') || (c >= '0' && c <= '9'))
		return (1);
	return (0);
}

int	valid_start_char(char c)
{
	if (c >= 'A' && c <= 'Z' || c >= 'a' && c <= 'z' || c == '_')
		return (1);
	return (0);
}

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
			if (process_status_str(exp_text, status_str) == -1)		// Append the status string to the expanded_text string.
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

// Appends the status string to the current exp_text
int	process_status_str(char **exp_text, const char *status_str)
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


