/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tok_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:48:25 by wheino            #+#    #+#             */
/*   Updated: 2025/10/07 12:48:53 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * SCANS FOR THE END OF A WORD SEGMENT STARTING AT INDEX i.
 * Inside quotes, consumes all characters until the matching closing quote.
 * Outside quotes, stops at a space, tab, or an operator (|, <, >, <<, >>).
 * Updates *in_single / *in_double as it passes opening/closing quotes.
 * If string is NULL, returns i unchanged.
 * @return Index after word end.
 */
int	scan_word_end(const char *string, int i, int *in_single, int *in_double)
{
	if (!string)
		return (i);
	while (string[i] != '\0')
	{
		if (!*in_single && !*in_double)
		{
			if (string[i] == ' ' || string[i] == '\t'
				|| get_operator_len(&string[i]) > 0)
				break ;
			if (string[i] == '\'')
				*in_single = 1;
			if (string[i] == '\"')
				*in_double = 1;
			i++;
		}
		else
		{
			if (*in_single && string[i] == '\'')
				*in_single = 0;
			else if (*in_double && string[i] == '\"')
				*in_double = 0;
			i++;
		}
	}
	return (i);
}

/**
 * CHECKS IF CURRENT SEGMENT IS AN OPERATOR.
 * IF OPERATOR - RETURNS THE OPERATORS LENGTH.
 * @return 1 / 2 if operator segment, 0 if not an operator segment. 
 */
int	get_operator_len(const char *string)
{
	if (!string || !*string)
		return (0);
	if (*string == '|')
		return (1);
	if (*string == '<')
	{
		if (string[1] == '<')
			return (2);
		return (1);
	}
	if (*string == '>')
	{
		if (string[1] == '>')
			return (2);
		return (1);
	}
	return (0);
}

/**
 * APPENDS A TOKEN NODE TO THE TEMPORARY LINKED LIST.
 * Creates a t_toknode and fills its fields.
 * The node takes ownership of 'text'.
 * On allocation failure, frees 'text'.
 * @return 0 on success, -1 on error (NULL args or malloc failure).
 */
int	append_token_node(t_tokenizer_state *state, TokType type,
		char *text, int pos)
{
	t_toknode	*node;

	if (!state || !text)
		return (-1);
	node = malloc(sizeof(t_toknode));
	if (!node)
	{
		free(text);
		return (-1);
	}
	node->tok.type = type;
	node->tok.text = text;
	node->tok.pos = pos;
	node->tok.was_quoted = 0;
	node->next = NULL;
	if (!state->head)
		state->head = node;
	else
		state->tail->next = node;
	state->tail = node;
	state->list_count += 1;
	return (0);
}

/**
 * Returns the enum TokType of the passed in string.
 * @return Operator type as TokType enum.
 */
TokType	get_operator_type(const char *string, int len)
{
	if (len == 1)
	{
		if (string[0] == '|')
			return (TOK_PIPE);
		if (string[0] == '<')
			return (TOK_REDIR_IN);
		if (string[0] == '>')
			return (TOK_REDIR_OUT);
	}
	else if (len == 2)
	{
		if (string[0] == '<' && string[1] == '<')
			return (TOK_HEREDOC);
		if (string[0] == '>' && string[1] == '>')
			return (TOK_REDIR_APP);
	}
	return (TOK_WORD);
}

int	skip_blanks(const char *string, int i)
{
	if (!string)
		return (i);
	while (string[i] == ' ' || string[i] == '\t')
		i++;
	return (i);
}
