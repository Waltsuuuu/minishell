#include "minishell.h"

/**
 * @brief Creates structured 't_input' from the given 'line'.
 * Stores 'line' as input->raw and parses the line into tokens.
 * Tokens are stored in the input->tokens array.
 * The number of tokens extracted is stored as input->n_tokens.
 *
 * @return 0 on success.
 * @return -1 if `line` or `input` is NULL, or if memory allocation fails. 
 */
int	parse_input_line(const char *line, t_input *input)
{
	if (!line || !input)
		return (-1);
	input_struct_zero(input);
	input->raw = ft_strdup(line);
	if (!input->raw)
		return (-1);
	if (tokenize_line(input->raw, input) == -1)
		return (clear_struct_on_failure(input));
	return (0);
}

void	input_struct_zero(t_input *input)
{
	if (!input)
		return ;
	input->raw = NULL;
	input->tokens = NULL;
	input->n_tokens = 0;
}

/**
 * @brief Frees all memory inside a t_input struct,
 * and sets all fields to 0 / NULL.
 * @return Always returns -1.
 */
int	clear_struct_on_failure(t_input *input)
{
	if (!input)
		return (-1);
	if (input->raw)
		free(input->raw);
	free_tokens(input);
	input_struct_zero(input);
	return (-1);
}

/**
 * @brief Frees the t_token array inside t_input.
 * Frees each tokens token.text and sets n_tokens to 0.
 * @return void. 
 */
void	free_tokens(t_input *input)
{
	int	i;

	if (!input || !input->tokens)
		return ;
	i = 0;
	while (i < input->n_tokens)
	{
		free(input->tokens[i].text);
		i++;
	}
	free(input->tokens);
	input->tokens = NULL;
	input->n_tokens = 0;
}
