/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/19 15:51:09 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/28 11:31:14 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned char	*end_ptr;
	unsigned char	*start_ptr;

	start_ptr = (unsigned char *)s;
	end_ptr = (unsigned char *)s + ft_strlen(s);
	while (end_ptr >= start_ptr)
	{
		if (*end_ptr == (unsigned char)c)
			return ((char *)end_ptr);
		end_ptr--;
	}
	return (NULL);
}
