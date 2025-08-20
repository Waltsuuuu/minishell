/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:53:00 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/29 12:18:19 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t		counter;
	size_t		dest_len;
	size_t		src_len;
	const char	*src_temp;
	const char	*dest_temp;

	src_temp = src;
	dest_temp = dst;
	dest_len = ft_strlen(dest_temp);
	src_len = ft_strlen(src_temp);
	counter = 0;
	if (size <= dest_len)
		return (size + src_len);
	else
	{
		while ((counter < (size - dest_len - 1)) && src[counter] != '\0')
		{
			dst[dest_len + counter] = src[counter];
			counter++;
		}
		dst[dest_len + counter] = '\0';
	}
	return (dest_len + src_len);
}
