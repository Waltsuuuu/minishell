/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 14:22:56 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/04/29 12:20:29 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(long n)
{
	int	count;

	count = 0;
	if (n == 0)
		return (1);
	while (n > 0)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	char	*result;
	long	dup_n;
	int		num_count;
	int		sign;

	dup_n = (long)n;
	sign = 0;
	if (dup_n < 0)
	{
		sign = 1;
		dup_n = -dup_n;
	}
	num_count = count_digits(dup_n);
	result = malloc(sizeof(char) * (num_count + sign + 1));
	if (!result)
		return (NULL);
	result[num_count + sign] = '\0';
	while (num_count > 0)
	{
		result[num_count-- + sign - 1] = (dup_n % 10) + '0';
		dup_n = dup_n / 10;
	}
	if (sign == 1)
		result[0] = '-';
	return (result);
}
