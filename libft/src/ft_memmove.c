/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 10:09:05 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/27 13:51:48 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	size_t			counter;
	unsigned char	*s;
	unsigned char	*d;

	s = (unsigned char *)src;
	d = (unsigned char *)dest;
	if ((d > s) && (d < (s + n)))
	{
		counter = n;
		while (counter > 0)
		{
			counter--;
			d[counter] = s[counter];
		}
		return (dest);
	}
	else
	{
		counter = 0;
		while (counter++ < n)
			d[counter - 1] = s[counter - 1];
		return (dest);
	}
}
