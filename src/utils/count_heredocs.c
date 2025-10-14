/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_heredocs.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 18:21:18 by wheino            #+#    #+#             */
/*   Updated: 2025/10/08 18:21:19 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	count_heredocs(t_input *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (i < input->n_tokens)
	{
		if (input->tokens[i].type == TOK_HEREDOC)
			count++;
		i++;
	}
	return (count);
}
