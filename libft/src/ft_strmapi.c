/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:17:08 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/28 12:51:32 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	size_t			size;
	unsigned char	*new_str;
	size_t			counter;

	if (!s || !f)
		return (NULL);
	size = ft_strlen(s);
	new_str = (unsigned char *)malloc(size + 1);
	if (!new_str)
		return (NULL);
	counter = 0;
	while (s[counter])
	{
		new_str[counter] = f(counter, s[counter]);
		counter++;
	}
	new_str[counter] = '\0';
	return ((char *)new_str);
}
