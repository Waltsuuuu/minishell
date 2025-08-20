/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_safe_atoi.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 13:12:08 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/06/19 11:37:06 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "push_swap.h"

int	ft_safe_atoi(const char *nptr, int *output)
{
	int		sign;
	long	result;

	sign = 1;
	result = 0;
	if (*nptr == '-' || *nptr == '+')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	if (!*nptr)
		return (0);
	while ((*nptr >= '0') && (*nptr <= '9'))
	{
		result = result * 10 + (*nptr - '0');
		if ((sign == 1 && result > INT_MAX)
			|| (sign == -1 && result > (long)INT_MAX + 1))
			return (0);
		nptr++;
	}
	if (*nptr != '\0')
		return (0);
	*output = (int)(sign * result);
	return (1);
}
