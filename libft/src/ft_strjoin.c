/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:19:02 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/29 13:30:30 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	s1_len;
	size_t	s2_len;
	char	*result_str;
	size_t	counter;

	if (!s1 || !s2)
		return (NULL);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	result_str = malloc(s1_len + s2_len + 1);
	if (result_str == NULL)
		return (NULL);
	counter = 0;
	while (*s1)
		result_str[counter++] = *s1++;
	while (*s2)
		result_str[counter++] = *s2++;
	result_str[counter] = '\0';
	return (result_str);
}
