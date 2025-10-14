/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 14:14:38 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/27 14:00:54 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *s, int c, size_t n)
{
	unsigned char	*temp;
	size_t			counter;

	temp = (unsigned char *)s;
	counter = 0;
	while (counter < n)
	{
		temp[counter] = (unsigned char)c;
		counter++;
	}
	return (s);
}
