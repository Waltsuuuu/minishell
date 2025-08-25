/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:14:50 by wheino            #+#    #+#             */
/*   Updated: 2025/08/25 14:32:11 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	char	*norm;

	if (!line || !input)
		return (-1);
	input_struct_zero(input);
	input->raw = ft_strdup(line);
	if (!input->raw)
		return (-1);
	norm = normalize_tabs(line);
	if (!norm)
		return (clear_struct_on_failure(input));
	input->words = ft_split(norm, ' ');
	if (!input->words)
	{
		free(norm);
		return (clear_struct_on_failure(input));
	}
	free(norm);
	input->count = count_words(input->words);
	if (input->count == 0)
	{
		free(input->words);
		input->words = NULL;
	}
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
	input->words = NULL;
	input->count = 0;
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
	if (input->words)
	{
		while (input->words[i] != NULL)
		{
			free(input->words[i]);
			i++;
		}
		free(input->words);
	}
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

/**
 * Creates a copy of a string with all tab characters replaced by spaces.
 *
 * @param line  The input string to normalize. Must not be NULL.
 *
 * @return	A newly allocated string identical to `line`,
 * 			but with tabs replaced by spaces.
 * 
 * @return NULL if `line` is NULL or if memory allocation fails.
 *
 */
char	*normalize_tabs(const char *line)
{
	char	*norm;
	int		i;

	norm = NULL;
	i = 0;
	if (!line)
		return (NULL);
	norm = ft_strdup(line);
	if (!norm)
		return (NULL);
	while (norm[i] != '\0')
	{
		if (norm[i] == '\t')
			norm[i] = ' ';
		i++;
	}
	return (norm);
}

/**
 * Counts the number of words in a NULL-terminated string array.
 *
 * @param words  A NULL-terminated array of strings
 *               If NULL, the function returns 0.
 *
 * @return The number of words (non-NULL elements) in the array.
 * 
 * @return 0 if `words` is NULL or the array is empty.
 *
 */
int	count_words(char **words)
{
	int	count;

	count = 0;
	if (!words)
		return (0);
	while (words[count] != NULL)
		count++;
	return (count);
}
