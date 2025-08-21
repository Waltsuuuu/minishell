/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:14:50 by wheino            #+#    #+#             */
/*   Updated: 2025/08/21 12:36:58 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * Parses a raw input line into a structured input representation.
 *
 * This function initializes the given `t_input` struct, stores a copy of 
 * the raw input string, normalizes it by replacing tabs with spaces, 
 * splits it into words, and counts the number of words. 
 * If any step fails, the input struct is cleared to avoid memory leaks.
 *
 * @param line   The raw input string to parse. Must not be NULL.
 * @param input  Pointer to a `t_input` struct that will be filled with:
 *               - `raw`: copy of the original input string.
 *               - `words`: array of split words (NULL if empty or on failure).
 *               - `count`: number of words in the array.
 *
 * @return 0 on success.
 * @return -1 if `line` or `input` is NULL, or if memory allocation fails.
 *
 * @note
 * - Tabs in the input are converted to spaces for consistent splitting.
 * - On failure, all allocated memory inside `input` is freed.
 * - If no words are found, `input->words` is set to NULL and `count` is 0.
 *
 * @see input_struct_init(), ft_strdup(), normalize_tabs(), ft_split(), 
 *      count_words(), clear_struct_on_failure()
 */
int	parse_input_line(const char *line, t_input *input)
{
	char	*norm;

	if (!line || !input)
		return (-1);
	input_struct_init(input);
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
 * Initializes a `t_input` structure to a safe default state.
 *
 * This function sets all fields of the provided `t_input` struct 
 * to their default values:
 * - `raw`   = NULL
 * - `words` = NULL
 * - `count` = 0
 *
 * This ensures the structure is ready to be safely used by parsing 
 * functions and prevents undefined behavior when freeing or accessing
 * uninitialized fields.
 *
 * @param input  Pointer to a `t_input` struct to initialize.
 *               If NULL, the function does nothing.
 *
 * @return void
 *
 * @note Always call this before populating a `t_input` structure.
 */
void	input_struct_init(t_input *input)
{
	if (!input)
		return ;
	input->raw = NULL;
	input->words = NULL;
	input->count = 0;
}

/**
 * Frees and resets all fields of a `t_input` structure after failure.
 *
 * This function is intended to be called when parsing or allocation 
 * fails. It safely frees any dynamically allocated memory stored in 
 * the `t_input` struct, and resets all fields to their default state:
 * - `raw`   = NULL
 * - `words` = NULL
 * - `count` = 0
 *
 * @param input  Pointer to a `t_input` struct to clear. 
 *               If NULL, no action is taken and -1 is returned.
 *
 * @return Always returns -1, to propagate failure codes from calling functions.
 *
 * @note
 * - Each element in `input->words` is freed before freeing the array itself.
 * - This function ensures the struct is left in a consistent, empty state,
 *   preventing double frees or dangling pointers.
 *
 * @see input_struct_init(), parse_input_line()
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
	input->raw = NULL;
	input->words = NULL;
	input->count = 0;
	return (-1);
}

/**
 * Creates a copy of a string with all tab characters replaced by spaces.
 *
 * This function duplicates the given input string and scans through it,
 * replacing each tab character ('\t') with a single space (' '). 
 * The returned string must be freed by the caller.
 *
 * @param line  The input string to normalize. Must not be NULL.
 *
 * @return	A newly allocated string identical to `line`,
 * 			but with tabs replaced by spaces.
 * @return NULL if `line` is NULL or if memory allocation fails.
 *
 * @note
 * - The returned string is allocated with `ft_strdup()` and
 * 	 must be freed by the caller.
 * - If no tab characters are found, the returned string is an
 * 	 exact duplicate of `line`.
 *
 * @see ft_strdup(), parse_input_line()
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
 * This function iterates through the given array of strings until a 
 * NULL pointer is encountered, returning the total number of elements.
 *
 * @param words  A NULL-terminated array of strings
 *               If NULL, the function returns 0.
 *
 * @return The number of words (non-NULL elements) in the array.
 * @return 0 if `words` is NULL or the array is empty.
 *
 * @note
 * - The function does not check the contents of the strings, only counts
 *   how many pointers are present before the terminating NULL.
 *
 * @see ft_split(), parse_input_line()
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
