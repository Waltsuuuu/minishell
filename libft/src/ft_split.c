/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:21:40 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/29 12:47:14 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	count_words(char const *s, char c)
{
	size_t	count_words;

	count_words = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s && *s != c)
			count_words++;
		while (*s != c && *s != '\0')
			s++;
	}
	return (count_words);
}

static	void	free_arr(char **arr)
{
	char	**start;

	start = arr;
	while (*arr)
	{
		free (*arr);
		arr++;
	}
	free(start);
}

static	size_t	word_len(const char *s, char deliminator)
{
	int	counter;

	counter = 0;
	while (*s)
	{
		if (*s == deliminator)
			return (counter);
		counter++;
		s++;
	}
	return (counter);
}

char	**ft_split(char const *s, char c)
{
	char	**strings_arr;
	size_t	i;

	strings_arr = malloc((count_words(s, c) + 1) * (sizeof(char *)));
	i = 0;
	if (!strings_arr)
		return (NULL);
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			strings_arr[i] = ft_substr(s, 0, word_len(s, c));
			if (!strings_arr[i])
			{
				free_arr(strings_arr);
				return (NULL);
			}
			i++;
			s = s + word_len(s, c);
		}
	}
	strings_arr[i] = NULL;
	return (strings_arr);
}
