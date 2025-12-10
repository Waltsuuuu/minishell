/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/20 15:35:05 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/28 12:42:08 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	const char	*temp_big;
	const char	*temp_little;
	size_t		little_len;
	size_t		temp_len;

	little_len = ft_strlen(little);
	if (*little == '\0')
		return ((char *)big);
	while (*big && (len >= little_len))
	{
		temp_len = len;
		temp_big = big;
		temp_little = little;
		while ((*temp_little && *temp_big == *temp_little) && (temp_len > 0))
		{
			temp_len--;
			temp_big++;
			temp_little++;
		}
		if (*temp_little == '\0')
			return ((char *)big);
		big++;
		len--;
	}
	return (NULL);
}
