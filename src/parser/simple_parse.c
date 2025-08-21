/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simple_parse.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/20 17:14:50 by wheino            #+#    #+#             */
/*   Updated: 2025/08/20 18:30:24 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
**	Takes a line from readline(), fills t_input struct fields (raw, words, count).
**		1. Sets the struct fields to NULL / 0.
**		2. Sets the raw field -> Original string.
**		3. Normalizes the whitespace of the string to spaces.
**		4. Splits the normalized string into words, using space as the delimiter.
**		5. Counts the number of words.
**	RETURNS:
**		0 	= SUCCESS
**		-1 	= FAILURE
*/
int parse_input_line(const char *line, t_input *input)
{
	char	*norm;								// Copy of riginal input string, with tabs changed to space for ft_split to work.
	
	if (!line || !input)
		return (-1);
	input_struct_init(input);					// Initialize the t_input struct to NULL / 0.
	input->raw = ft_strdup(line);				// Set the input struct 'raw' field = To the original input line
	if (!input->raw)
		return (-1);
	norm = normalize_tabs(line);				// Normalie the input string -> Changes tabs to spaces (if any).
	if (!norm)
		return (clear_struct_on_failure(input));
	input->words = ft_split(norm, ' ');			// Split the normalized string words into seperate strings.
	if (!input->words)
	{
		free(norm);
		return (clear_struct_on_failure(input));
	}
	free(norm);									// Free the normalized string.
	input->count = count_words(input->words);	// Count the words and set t_input count field.
	if (input->count == 0)
	{
		free(input->words);
		input->words = NULL;
	}
	return (0);
}
/*	Zero / NULL initialize the input struct  */
void	input_struct_init(t_input *input)
{
	if (!input)
		return ;
	input->raw = NULL;
	input->words = NULL;
	input->count = 0;
}

/*	*/
int	clear_struct_on_failure(t_input *input)
{
	int	i;

	i = 0;
	if (!input)
		return(-1);
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
/*	Changes the input strings tabs to spaces  */
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

/*	Count the number of words in words array  */
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