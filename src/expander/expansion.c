#include "minishell.h"
#include <bsd/string.h>

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

size_t	copy_n_chars(char *dst, const char *src, size_t size)
{
	size_t	counter;
	size_t	src_len;

	src_len = ft_strlen(src);
	if (size != 0)
	{
		counter = 0;
		while ((src[counter] != '\0') && (counter < size))
		{
			dst[counter] = src[counter];
			counter++;
		}
		dst[counter] = '\0';
	}
	return (src_len);
}