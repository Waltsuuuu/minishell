#include "minishell.h"

/**
 * Parses a raw input line into a structured input representation.
 *
 * @param line   The raw input string to parse. Must not be NULL.
 * 
 * @param input  Pointer to a `t_input` struct that will be filled.
 *
 * @return 0 on success.
 * 
 * @return -1 if `line` or `input` is NULL, or if memory allocation fails.
 *
 */
int	parse_input_line(const char *line, t_input *input)
{
	if (!line || !input)
		return (-1);
	input_struct_zero(input);
	input->raw = ft_strdup(line);
	if (!input->raw)
		return (-1);
	if (tokenize_line(input->raw, &input->tokens, &input->n_tokens) == -1)
		return (clear_struct_on_failure(input));
	return (0);
}

/**
 * Initializes a `t_input` structure fields to 0 / NULL.
 *
 * @param input  Pointer to a `t_input` struct to initialize.
 *               If NULL, the function does nothing.
 *
 * @return void
 *
 */
void	input_struct_zero(t_input *input)
{
	if (!input)
		return ;
	input->raw = NULL;
	input->tokens = NULL;
	input->n_tokens = 0;
}

/**
 * Frees and resets all fields of a `t_input` structure after failure.
 *
 * @param input  Pointer to a `t_input` struct to clear. 
 *               If NULL, no action is taken and -1 is returned.
 *
 * @return Always returns -1.
 *
 */
int	clear_struct_on_failure(t_input *input)
{
	int	i;

	i = 0;
	if (!input)
		return (-1);
	if (input->raw)
		free(input->raw);
	free_tokens(input);
	input_struct_zero(input);
	return (-1);
}
/**
 * Frees all tokens stored in a `t_input` structs token array.
 *
 * @param input  Pointer to the `t_input` whose tokens will be freed.
 *               If NULL, or if `input->tokens` is NULL, the function does nothing.
 *
 * @return void
 *
 */
void	free_tokens(t_input *input)
{
	int i;
	
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

