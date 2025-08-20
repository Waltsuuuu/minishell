/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 10:08:48 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/20 17:53:18 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "colors.h"

typedef struct s_input {
	char	*raw;		// Original line the user typed in.
	char	**words;		// Array of words (split from the original line).
	int		count;		// Number of words.
}		t_input;

void	getworkindir(char *buf, size_t size);

#endif