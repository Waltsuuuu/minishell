#include "minishell.h"

/**
 * Ensures the quotes in the input line are balanced. If no closing quote is found
 * user is given a continuation prompt > where they can continue entering input.
 * Updates *line if new input is received. On Ctrl-D/malloc error: frees *line, returns -1.
 * @return 0 on success, -1 on failure.
 */
int check_quote_balance(char **line)
{
	char	*new_input;

	if (!line || !*line)
		return (-1);
	while (quotes_unbalanced(*line))
	{
		new_input = readline("> ");
		if (!new_input)
		{
			write(2, "minishell: unexpected EOF while looking for matching quote\n", 64);
			free(*line);
			*line = NULL;
			return (-1);
		}
		if (append_new_input(line, new_input) == -1)
		{
			free(new_input);
			return (-1);
		}
		free(new_input);
	}
	return (0);
}

/**
 * Check if single/double quotes are unbalanced in string.  
 * @return 1 if unbalanced, 0 if balanced (NULL is also treated as balanced). 
 */
int	quotes_unbalanced(const char *string)
{
	int	in_single;	
	int	in_double;
	int	i;

	if (!string)
		return (0);
	in_single = 0;
	in_double = 0;
	i = 0;
	while (string[i] != '\0')
	{
		if (string[i] == '\'' && !in_double)		// If single quote is found and we arent inside a double quote;
			in_single = !in_single;					// Turn in_single on. // Turned off when closing quote is found
		else if (string[i] == '\"' && !in_single)	
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);				// Returns 1 if either flag is still true. 0 if both are false
}

// Like strjoin, but adds a newline between the line + new_input.
int	append_new_input(char **line, char *new_input)
{
	char	*temp;
	char	*join;

	if (!line || !*line || !new_input)
		return (-1);
	temp = ft_strjoin(*line, "\n");
	if (!temp)
		return (-1);
	join = ft_strjoin(temp, new_input);
	free(temp);
	if (!join)
		return (-1);
	*line = join;
	return (0);
}
