/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 14:49:50 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/28 13:19:00 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	isset(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	unsigned int		end_point;
	char				*new_string;
	unsigned int		start_counter;

	end_point = ft_strlen(s1);
	start_counter = 0;
	if (!s1 || !set)
		return (NULL);
	while (isset(s1[start_counter], set))
		start_counter++;
	while ((end_point > start_counter) && isset(s1[end_point - 1], set))
		end_point--;
	new_string = ft_substr(s1, start_counter, end_point - start_counter);
	return (new_string);
}
