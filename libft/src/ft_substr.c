/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:41:34 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/28 13:19:38 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	char	*sub_string;
	size_t	str_len;
	size_t	counter;

	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup(""));
	if (len > str_len - start)
		len = str_len - start;
	sub_string = malloc(len + 1);
	if (sub_string == NULL)
		return (NULL);
	counter = 0;
	while (counter < len)
	{
		sub_string[counter] = s[start];
		counter++;
		start++;
	}
	sub_string[counter] = '\0';
	return (sub_string);
}
