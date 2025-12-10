/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/08 12:52:37 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/05/20 15:21:18 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*strjoin_gnl(char *s1, char *s2)
{
	size_t	total_len;
	size_t	counter;
	char	*result_str;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (strdup_gnl(s2));
	else if (!s2)
		return (strdup_gnl(s1));
	total_len = (strlen_gnl(s1) + strlen_gnl(s2));
	result_str = malloc(total_len + 1);
	if (!result_str)
		return (free(s1), NULL);
	counter = 0;
	while (s1[counter])
	{
		result_str[counter] = s1[counter];
		counter++;
	}
	while (*s2)
		result_str[counter++] = *s2++;
	result_str[counter] = '\0';
	free(s1);
	return (result_str);
}

char	*strchr_gnl(const char *s, int c)
{
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (unsigned char)c)
			return ((char *)s);
		s++;
	}
	if (*s == (unsigned char)c)
		return ((char *)s);
	return (NULL);
}

size_t	strlen_gnl(const char *s)
{
	size_t	counter;

	counter = 0;
	while (*s)
	{
		s++;
		counter++;
	}
	return (counter);
}

size_t	strlcpy_gnl(char *dst, const char *src, size_t size)
{
	size_t	counter;
	size_t	src_len;

	src_len = strlen_gnl(src);
	if (size != 0)
	{
		counter = 0;
		while ((src[counter] != '\0') && (counter < size - 1))
		{
			dst[counter] = src[counter];
			counter++;
		}
		dst[counter] = '\0';
	}
	return (src_len);
}

char	*strdup_gnl(const char *s)
{
	char	*dup_str;
	size_t	counter;

	dup_str = malloc(strlen_gnl(s)+1);
	if (dup_str == NULL)
		return (NULL);
	counter = 0;
	while (s[counter] != '\0')
	{
		dup_str[counter] = s[counter];
		counter++;
	}
	dup_str[counter] = '\0';
	return (dup_str);
}
