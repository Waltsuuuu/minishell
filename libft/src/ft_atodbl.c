/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atodbl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/06 13:47:19 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/08/14 09:01:44 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "fractol.h"

double	ft_atodbl(char *str)
{
	double	n;
	double	multiplier;
	int		sign;

	sign = 1;
	n = 0;
	multiplier = 10;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		str++;
	if (*str == '-' || *str == '+')
	{
		if (*str == '-')
			sign = -1;
		str++;
	}
	while (*str && *str != '.')
		n = n * 10 + (*str++ - '0');
	if (*str == '.')
		str++;
	while (*str)
	{
		n += (*str++ - '0') / multiplier;
		multiplier *= 10;
	}
	return (n * sign);
}
