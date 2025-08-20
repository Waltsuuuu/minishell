/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 13:28:53 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/29 12:18:06 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*dup_str;
	size_t	counter;

	dup_str = malloc(ft_strlen(s)+1);
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
