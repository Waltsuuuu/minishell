/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_expand_variable.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:27:38 by wheino            #+#    #+#             */
/*   Updated: 2025/10/07 17:22:58 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Rebuilds token.text, expanding $VAR (KEY) to its value.
 * If KEY is invalid, it is appended as it is.
 * If KEY is valid but is not declared, KEY is expanded to
 * an empty string.
 * While not inside quotes, a '$' preceeding a quoted segment is skipped.
 * No expansion inside single quotes.
 * @return Newly allocated expanded string, or NULL on error.
 */
char	*expand_variable(char *text, char **envp)
{
	char	*exp_text;

	if (!text)
		return (ft_strdup(""));
	exp_text = ft_strdup("");
	if (!exp_text)
		return (NULL);
	if (create_exp_var_text(text, &exp_text, envp) == -1)
	{
		free(exp_text);
		return (NULL);
	}
	return (exp_text);
}

/**
 * @brief Iterates over token.text, rebuilds the string while searching for
 * a valid "$VAR" segment to expand.
 * Keeps track of quote state. Does not expand inside single quotes.
 * @return 0 on success, -1 on error.
 */
int	create_exp_var_text(char *text, char **exp_text, char **envp)
{
	t_expand_state	st;

	init_expand_state(&st);
	while (text[st.i] != '\0')
	{
		st.quote_handled = process_quote_char(text[st.i], &st.in_single,
				&st.in_double, exp_text);
		if (st.quote_handled == -1)
			return (-1);
		if (st.quote_handled == 1)
		{
			st.i++;
			continue ;
		}
		st.expanded = handle_var_expansion(&st, text, exp_text, envp);
		if (st.expanded == -1)
			return (-1);
		if (st.expanded == 1)
			continue ;
		if (process_char(exp_text, text[st.i]) == -1)
			return (-1);
		st.i++;
	}
	return (0);
}

/**
 * @brief Checks if we are dealing with a valid $VAR (KEY) segment.
 * If valid: Expand $VAR to its value and append it to the string.
 * Valid: $ followed by a '_' or alpha char.
 * @note:
 * - While not inside quotes, a '$' preceeding a quoted segment is skipped.
 * - Increments the iterator as needed.
 * @return 1 handled, 0 if invalid segment, -1 on error.
 */
int	handle_var_expansion(t_expand_state *st, char *text,
		char **exp_text, char **envp)
{
	int	status;

	if (!st->in_single && !st->in_double
		&& text[st->i] == '$' && text[st->i + 1] == '\"')
	{
		st->i++;
		return (1);
	}
	if (!st->in_single && !st->in_double && text[st->i] == '$'
		&& text[st->i + 1] == '\'')
	{
		st->i++;
		return (1);
	}
	status = 0;
	if (!st->in_single && text[st->i] == '$'
		&& valid_start_char(text[st->i + 1]) == 1)
	{
		st->i++;
		status = process_var_expansion(text, exp_text, &st->i, envp);
		if (status == -1)
			return (-1);
		return (1);
	}
	return (0);
}

/**
 * @brief Extracts the $VAR KEY and searches for a match in the
 * environment array.
 * If match found, extract the VALUE and append it to the exp_text string.
 * If no match found, the "$VAR" segment is skipped ("Expands to empty").
 * @return 1 if expanded, 0 if KEY is not declared, -1 on error.
 */
int	process_var_expansion(char *text, char **exp_text, int *i, char **envp)
{
	char	*key;
	int		key_len;
	int		start_i;
	int		env_i;

	key = extract_key(text, i, &key_len, &start_i);
	if (!key)
		return (-1);
	env_i = find_env_index(envp, key, key_len);
	if (env_i >= 0)
	{
		if (process_expanded_str(exp_text, &envp[env_i][key_len + 1]) == -1)
		{
			free(key);
			return (-1);
		}
		free(key);
		return (1);
	}
	free(key);
	return (0);
}
