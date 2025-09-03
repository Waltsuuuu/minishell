#include "minishell.h"

int	remove_quotes(t_input *input)
{
	int		i;
	char	*unquoted_text;

	if (!input || !input->tokens)
		return (-1);
	i = 0;
	while (i < input->n_tokens)						
	{
		if (input->tokens[i].type == TOK_WORD)
		{
			unquoted_text = handle_quote_removal(input->tokens[i].text);
			if (!unquoted_text)
				return (-1);
			free(input->tokens[i].text);
			input->tokens[i].text = unquoted_text;
		}
		i++;
	}
	return (0);
}

char *handle_quote_removal(char *text)
{
	char	*unquoted_text;

	if (!text)
		return (ft_strdup(""));
	unquoted_text = ft_strdup("");
	if (!unquoted_text)
		return (NULL);
	if (create_unquoted_text(text, &unquoted_text) == -1)
	{
		free(unquoted_text);
		return (NULL);
	}
	return (unquoted_text);
}

int	create_unquoted_text(char *text, char **unquoted_text)
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
		quote_removed = remove_outer_quote(text[i], &in_single, &in_double);
		if (quote_removed == 1)	// 1 quote removed
		{
			i++;
			continue ;
		}							// 0 not a quote char / nested quote.
		if (process_char(unquoted_text, text[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
}

int	remove_outer_quote(char c, int *in_single, int *in_double)
{
	if (c == '\'' && !*in_double)
	{
		*in_single = !*in_single;
		return (1);
	}
	if (c == '\"' && !*in_single)
	{
		*in_double = !*in_double;
		return (1);
	}
	return 0;
}
