/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_expand_status.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:54:41 by wheino            #+#    #+#             */
/*   Updated: 2025/10/07 13:45:25 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Rebuilds token.text, expanding $? to the
 * last_status.
 * @return Newly allocated expanded string, or NULL on error.
 */
char	*expand_status(char *text, int last_status)
{
	char	*exp_text;
	char	*status_str;

	if (!text)
		return (ft_strdup(""));
	exp_text = ft_strdup("");
	if (!exp_text)
		return (NULL);
	status_str = ft_itoa(last_status);
	if (!status_str)
	{
		free(exp_text);
		return (NULL);
	}
	if (create_exp_status_text(text, &exp_text, status_str) == -1)
	{
		free(status_str);
		free(exp_text);
		return (NULL);
	}
	free(status_str);
	return (exp_text);
}

/**
 * @brief Iterates over token.text, rebuilds the string while
 * searching for a "$?" segment to expand.
 * Keeps track of quote state. Does not expand inside single quotes.
 * @return 0 on success, -1 on error.
 */
int	create_exp_status_text(char *text, char **exp_text, char *status_str)
{
	t_expand_state	st;

	init_expand_state(&st);
	if (loop_rebuild_expand(text, exp_text, status_str, &st) == -1)
		return (-1);
	return (0);
}

int	loop_rebuild_expand(char *text, char **exp_text, char *status_str,
		t_expand_state *st)
{
	while (text[st->i] != '\0')
	{
		st->quote_handled = process_quote_char(text[st->i], &st->in_single,
				&st->in_double, exp_text);
		if (st->quote_handled == -1)
			return (-1);
		if (st->quote_handled == 1)
		{
			st->i++;
			continue ;
		}
		if (!st->in_single && text[st->i] == '$' && text[st->i + 1] == '?')
		{
			if (process_expanded_str(exp_text, status_str) == -1)
				return (-1);
			st->i += 2;
			continue ;
		}
		if (process_char(exp_text, text[st->i]) == -1)
			return (-1);
		st->i++;
	}
	return (0);
}
