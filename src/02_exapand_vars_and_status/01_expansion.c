/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_expansion.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 12:52:58 by wheino            #+#    #+#             */
/*   Updated: 2025/10/07 12:53:43 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Expand WORD tokens for exit status ($?),
 * and environment ($VAR) variables.
 * @return 0 on success; -1 on allocation/expansion error.
 */
int	expand_tokens(t_input *input, int last_status, char **envp)
{
	int		i;
	char	*exp_text;

	if (!input || !input->tokens)
		return (0);
	i = 0;
	while (i < input->n_tokens)
	{
		if ((input->tokens[i].type == TOK_WORD)
			&& (i == 0 || input->tokens[i - 1].type != TOK_HEREDOC))
		{
			exp_text = expand_status(input->tokens[i].text, last_status);
			if (!exp_text)
				return (-1);
			free(input->tokens[i].text);
			input->tokens[i].text = exp_text;
			exp_text = expand_variable(input->tokens[i].text, envp);
			if (!exp_text)
				return (-1);
			free(input->tokens[i].text);
			input->tokens[i].text = exp_text;
		}
		i++;
	}
	return (0);
}
