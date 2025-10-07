/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_check.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 13:31:55 by wheino            #+#    #+#             */
/*   Updated: 2025/10/06 13:31:56 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Ensures the quotes in the input line are balanced.
 * If no closing quote is found the input is discarded,
 * and an error message is written, last status is set to 2 (syntax error),
 * and user is returned to prompt.
 * @return 0 if balance, -1 if unbalanced. 
 */
int	check_quote_balance(char **line, int *last_status)
{
	if (!line || !*line)
		return (-1);
	if (quotes_unbalanced(*line))
	{
		ft_putstr_fd("minishell: unclosed quote\n", 2);
		free(*line);
		*line = NULL;
		*last_status = 2;
		return (-1);
	}
	return (0);
}

/**
 * Check if single/double quotes are unbalanced in string.  
 * @return 1 if unbalanced, 0 if balanced (NULL is also treated as balanced). 
 */
int	quotes_unbalanced(const char *string)
{
	int	in_single;	
	int	in_double;
	int	i;

	if (!string)
		return (0);
	in_single = 0;
	in_double = 0;
	i = 0;
	while (string[i] != '\0')
	{
		if (string[i] == '\'' && !in_double)
			in_single = !in_single;
		else if (string[i] == '\"' && !in_single)
			in_double = !in_double;
		i++;
	}
	return (in_single || in_double);
}
