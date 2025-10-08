/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtoll.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhirvasm <mhirvasm@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/08 08:53:57 by mhirvasm          #+#    #+#             */
/*   Updated: 2025/10/08 09:16:35 by mhirvasm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	read_optional_sign(const char **p_str, int *is_negative)
{
	const char	*s;

	s = *p_str;
	*is_negative = 0;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			*is_negative = 1;
		s++;
		if (!*s)
			return (0);
	}
	*p_str = s;
	return (1);
}

static int	parse_digits_neg(const char **p_str, long long *running_value)
{
	const char	*s;
	int			d;

	s = *p_str;
	while (*s >= '0' && *s <= '9')
	{
		d = *s - '0';
		if (*running_value < (LLONG_MIN + d) / 10)
			return (0);
		*running_value = (*running_value * 10) - d;
		s++;
	}
	*p_str = s;
	return (1);
}

int	ft_strtoll(const char *str, long long *out_value)
{
	int			is_negative;
	long long	running_value;

	if (!str || !out_value)
		return (0);
	if (!read_optional_sign(&str, &is_negative))
		return (0);
	if (*str < '0' || *str > '9')
		return (0);
	running_value = 0;
	if (!parse_digits_neg(&str, &running_value))
		return (0);
	if (*str != '\0')
		return (0);
	if (!is_negative)
	{
		if (running_value == LLONG_MIN)
			return (0);
		*out_value = -running_value;
	}
	else
		*out_value = running_value;
	return (1);
}
