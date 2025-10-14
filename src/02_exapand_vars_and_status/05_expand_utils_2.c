/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   05_expand_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wheino <wheino@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/07 14:54:51 by wheino            #+#    #+#             */
/*   Updated: 2025/10/07 17:22:48 by wheino           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_expand_state(t_expand_state *st)
{
	st->i = 0;
	st->in_single = 0;
	st->in_double = 0;
}

size_t	copy_n_chars(char *dst, const char *src, size_t size)
{
	size_t	counter;
	size_t	src_len;

	src_len = ft_strlen(src);
	if (size != 0)
	{
		counter = 0;
		while ((src[counter] != '\0') && (counter < size))
		{
			dst[counter] = src[counter];
			counter++;
		}
		dst[counter] = '\0';
	}
	return (src_len);
}

/**
 * @brief Extract $VAR (KEY) at *i and return malloc'ed "KEY=".
 * @return 0 on success, -1 on allocation error.
 */
char	*extract_key(char *text, int *i, int *key_len, int *start_i)
{
	char	*key;

	*start_i = *i;
	*key_len = 0;
	while (valid_cont_char(text[*i]) == 1)
	{
		(*i)++;
		(*key_len)++;
	}
	key = malloc(*key_len + 2);
	if (!key)
		return (NULL);
	copy_n_chars(key, &text[*start_i], *key_len);
	key[*key_len] = '=';
	key[*key_len + 1] = '\0';
	return (key);
}

/**
 * @brief Searches for matching KEY inside the environment array.
 * @return Index of matching key in environment array, -1 if no match found.
 */
int	find_env_index(char **envp, char *key, int key_len)
{
	int	env_i;

	env_i = 0;
	while (envp[env_i])
	{
		if (ft_strncmp(envp[env_i], key, key_len + 1) == 0)
			return (env_i);
		env_i++;
	}
	return (-1);
}
