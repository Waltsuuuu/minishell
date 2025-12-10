/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_expand_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 13:20:21 by wheino            #+#    #+#             */
/*   Updated: 2025/10/07 17:31:55 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Checks if we are dealing with a valid
 * env-var KEY character (but not the first char).
 * @return 1 if valid, 0 if invalid.
 */
int	valid_cont_char(char c)
{
	if (ft_isalpha(c) || (c == '_') || ft_isdigit(c))
		return (1);
	return (0);
}

/**
 * @brief Checks if we are dealing with a valid
 * env-var KEY starting character. Non valid names
 * are not taken into expansion.
 * @return 1 if valid, 0 if invalid.
 */
int	valid_start_char(char c)
{
	if (ft_isalpha(c) || (c == '_'))
		return (1);
	return (0);
}

/**
 * @brief Append the status_string to exp_text.
 * @param exp_text Destination string pointer.
 * @param str String to append.
 * @return 0 on success, -1 on error.
 */
int	append_expanded_str(char **exp_text, const char *str)
{
	char	*joined;

	if (!exp_text || !*exp_text || !str)
		return (-1);
	joined = ft_strjoin(*exp_text, str);
	if (!joined)
		return (-1);
	free(*exp_text);
	*exp_text = joined;
	return (0);
}

/**
 * @brief Checks if char is a quote and if it
 * should be appended to the rebuilt string or not,
 * based on the current quote state.
 * @return 1 if quote handled, 0 if not a quote, -1 on error.
 */
int	process_quote_char(char c, int *in_single, int *in_double, char **exp_text)
{
	if (c == '\'' && !*in_double)
	{
		*in_single = !*in_single;
		if (append_char(exp_text, '\'') == -1)
			return (-1);
		return (1);
	}
	if (c == '\"' && !*in_single)
	{
		*in_double = !*in_double;
		if (append_char(exp_text, '\"') == -1)
			return (-1);
		return (1);
	}
	return (0);
}

/**
 * @brief Appends a single char to the string,
 * keeping the string NULL-terminated.
 * @return 0 on success, -1 on error.
 */
int	append_char(char **exp_text, char c)
{
	char	buf[2];
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
