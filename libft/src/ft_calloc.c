/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 11:07:43 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/27 14:00:35 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	void	*ptr;
	size_t	mem_size;

	mem_size = nmemb * size;
	if (nmemb == 0 || size == 0)
		return (malloc(0));
	if (mem_size / nmemb != size)
		return (NULL);
	ptr = malloc(mem_size);
	if (ptr == NULL)
		return (NULL);
	ft_bzero(ptr, mem_size);
	return (ptr);
}
