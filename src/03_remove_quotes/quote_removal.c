/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_removal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 17:49:46 by wheino            #+#    #+#             */
/*   Updated: 2025/10/07 17:50:27 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Remove outer quotes from WORD tokens in-place.
 * If token.text is an empty string after quote removal,
 * change the TOK_TYPE to void.
 * @return 0 on success, -1 on error or invalid input.
 */
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
			unquoted_text = handle_quote_removal(input->tokens[i].text,
					&input->tokens[i].was_quoted);
			if (!unquoted_text)
				return (-1);
			free(input->tokens[i].text);
			input->tokens[i].text = unquoted_text;
			if (unquoted_text[0] == '\0' && input->tokens[i].was_quoted == 0)
				input->tokens[i].type = TOK_VOID;
		}
		i++;
	}
	return (0);
}

/**
 * @brief Creates a new string with outer quotes stripped.
 * @return Newly allocated unquoted string, or NULL on error.
 */
char	*handle_quote_removal(char *text, int *was_quoted)
{
	char	*unquoted_text;

	if (!text)
		return (ft_strdup(""));
	unquoted_text = ft_strdup("");
	if (!unquoted_text)
		return (NULL);
	if (create_unquoted_text(text, &unquoted_text, was_quoted) == -1)
	{
		free(unquoted_text);
		return (NULL);
	}
	return (unquoted_text);
}

/**
 * @brief Iterater over token.text, rebuilding the string char by char.
 * Outer quotes are skipped.
 * @return 0 on success, -1 on error.
 */
int	create_unquoted_text(char *text, char **unquoted_text, int *was_quoted)
{
	int	i;
	int	in_single;
	int	in_double;
	int	skip_quote;

	i = 0;
	in_double = 0;
	in_single = 0;
	while (text[i] != '\0')
	{
		skip_quote = detect_outer_quote(text[i], &in_single, &in_double);
		if (skip_quote == 1)
		{
			*was_quoted = 1;
			i++;
			continue ;
		}
		if (append_char(unquoted_text, text[i]) == -1)
			return (-1);
		i++;
	}
	return (0);
}

/**
 * @brief Detects if char is an outer quote..
 * @return 1 if an outer quote was detected, else 0.
 */
int	detect_outer_quote(char c, int *in_single, int *in_double)
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
	return (0);
}
